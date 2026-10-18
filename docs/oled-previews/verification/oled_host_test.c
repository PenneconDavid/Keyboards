#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define PROGMEM
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#include "../../../keyboards/crkbd/keymaps/keebv2/config.h"
#define KEEB_PET_STATE 1
#define MOD_MASK_CTRL 0x11
#define KC_SPC 44
#define IS_QK_LAYER_TAP(k) (((k)&0xf000)==0x4000)
#define QK_LAYER_TAP_GET_TAP_KEYCODE(k) ((k)&0xff)
typedef enum { OLED_ROTATION_0, OLED_ROTATION_90, OLED_ROTATION_180, OLED_ROTATION_270 } oled_rotation_t;
typedef struct { struct {bool pressed;} event; struct {uint8_t count;} tap;} keyrecord_t;
enum {_BASE, _SYM, _GAME_WASD, _FN, _ARC, _GAME_ESDF, _ARC_E};
static bool master=true, display_on=true, caps=false, rpc_ok=true;
static uint32_t now=1000, idle=0;
static uint8_t mods=0, weak=0, wpm=0, layer_state=0, default_layer_state=0;
static unsigned rpc_calls=0, write_calls=0;
static uint8_t framebuffer[512], last_packet[2];
static uint32_t timer_read32(void){return now;}
static uint32_t timer_elapsed32(uint32_t t){return now-t;}
static uint32_t last_input_activity_elapsed(void){return idle;}
static bool is_keyboard_master(void){return master;}
static uint8_t get_current_wpm(void){return wpm;}
static uint8_t get_mods(void){return mods;}
static uint8_t get_weak_mods(void){return weak;}
typedef struct {bool caps_lock;} led_t;
static led_t host_keyboard_led_state(void){return (led_t){caps};}
static bool transaction_rpc_send(int id, uint8_t size, const void *data){assert(id==1&&size==2);rpc_calls++;memcpy(last_packet,data,2);return rpc_ok;}
static void oled_off(void){display_on=false;}
static void oled_on(void){display_on=true;}
static void oled_write_raw_byte(uint8_t value,uint16_t index){assert(index<512);framebuffer[index]=value;write_calls++;}
typedef struct {uint8_t *current_element; uint16_t remaining_element_count;} oled_buffer_reader_t;
oled_buffer_reader_t oled_read_raw(uint16_t index){assert(index<512);return (oled_buffer_reader_t){framebuffer+index,512-index};}
static uint8_t get_highest_layer(uint8_t mask){uint8_t v=0;while(mask>>1){mask>>=1;v++;}return v;}
#include "../../../keyboards/crkbd/keymaps/keebv2/oled_graphics.h"
#include "luna_expected.h"
static void expect_band(uint8_t pose, bool jump){
  unsigned offset=jump?0:32; bool matched=false;
  for(unsigned f=0;f<2;f++){
    bool same=true;
    for(unsigned i=0;i<160;i++){
      uint8_t expected=(i>=offset&&i<offset+96)?expected_luna_frames[pose*2+f][i-offset]:0;
      if(framebuffer[352+i]!=expected)same=false;
    }
    matched|=same;
  }
  assert(matched);
}
int main(void){
  for(unsigned flip=0;flip<2;flip++){
    for(unsigned l=0;l<7;l++){
      memset(framebuffer,0xa5,512);
      decode_graphic(card_stream,(l+1)*512,512,0,flip);
      for(unsigned i=0;i<512;i++)assert(read_oled_byte(i,flip)==expected_cards[l][i]);
    }
    for(unsigned s=0;s<10;s++){
      memset(framebuffer,0xa5,512);
      decode_graphic(luna_stream,(s+1)*96,96,352,flip);
      for(unsigned i=0;i<96;i++)assert(read_oled_byte(352+i,flip)==expected_luna_frames[s][i]);
      for(unsigned i=0;i<352;i++)assert(read_oled_byte(i,flip)==0xa5);
      for(unsigned i=448;i<512;i++)assert(read_oled_byte(i,flip)==0xa5);
    }
  }
  memset(framebuffer,0,512);
  uint8_t cards[7][512];
  for(unsigned l=0;l<7;l++){layer_state=1u<<l;now+=100;oled_task_user();memcpy(cards[l],framebuffer,512);assert(!memcmp(cards[l],expected_cards[l],512));}
  for(unsigned a=0;a<7;a++)for(unsigned b=a+1;b<7;b++)assert(memcmp(cards[a],cards[b],512));
  assert(oled_init_user(OLED_ROTATION_0)==OLED_ROTATION_270);
  master=false;assert(oled_init_user(OLED_ROTATION_180)==OLED_ROTATION_270);
  const uint8_t speeds[3]={0,20,60};
  for(unsigned p=0;p<5;p++){
    pet_state_t incoming={p<3?speeds[p]:0,p==3?PET_CAPS:p==4?PET_CTRL:0};
    pet_receive(sizeof incoming,&incoming,0,NULL);now+=250;oled_task_user();expect_band(p,false);
  }
  pet_state_t incoming={60,PET_CAPS|PET_CTRL|PET_JUMP};pet_receive(2,&incoming,0,NULL);now+=250;oled_task_user();expect_band(3,true);
  incoming=(pet_state_t){0,0};pet_receive(2,&incoming,0,NULL);now+=250;oled_task_user();expect_band(0,false);
  unsigned before=write_calls;incoming.flags=PET_SLEEP;pet_receive(2,&incoming,0,NULL);oled_task_user();assert(!display_on&&before==write_calls);
  incoming.flags=0;pet_receive(2,&incoming,0,NULL);now+=250;oled_task_user();assert(display_on);expect_band(0,false);
  master=true;idle=60000;before=write_calls;oled_task_user();assert(!display_on&&before==write_calls);idle=0;oled_task_user();assert(display_on);
  keyrecord_t rec={.event={true},.tap={0}};
  oled_track_space(0x4000|KC_SPC,&rec);assert(!space_held&&!jump_latched);
  rec.tap.count=1;oled_track_space(0x4000|KC_SPC,&rec);assert(space_held&&jump_latched);
  rec.event.pressed=false;oled_track_space(0x4000|KC_SPC,&rec);assert(!space_held&&jump_latched);
  rpc_ok=false;now+=100;housekeeping_task_user();unsigned attempts=rpc_calls;assert(last_packet[1]&PET_JUMP);
  now+=100;housekeeping_task_user();assert(rpc_calls==attempts+1);
  rpc_ok=true;now+=100;housekeeping_task_user();assert(last_packet[1]&PET_JUMP);
  now+=200;housekeeping_task_user();assert(!(last_packet[1]&PET_JUMP));
  rec.event.pressed=true;oled_track_space(KC_SPC,&rec);now+=1000;housekeeping_task_user();assert(last_packet[1]&PET_JUMP);
  rec.event.pressed=false;oled_track_space(KC_SPC,&rec);now+=100;housekeeping_task_user();assert(!(last_packet[1]&PET_JUMP));
  mods=1;caps=true;idle=60000;now+=100;housekeeping_task_user();assert(last_packet[1]==(PET_CTRL|PET_CAPS|PET_SLEEP));
  attempts=rpc_calls;now+=100;housekeeping_task_user();assert(rpc_calls==attempts);now+=500;housekeeping_task_user();assert(rpc_calls==attempts+1);
  idle=0;mods=0;caps=false;now+=100;housekeeping_task_user();assert(!last_packet[1]);
  printf("PASS: seven distinct cards; rotation; five Luna poses; Caps precedence; jump and landing band cleanup; slave and master sleep/wake; LT Space tap/hold; RPC retries, state changes and heartbeat; framebuffer bounds (%u writes).\n",write_calls);
  return 0;
}
