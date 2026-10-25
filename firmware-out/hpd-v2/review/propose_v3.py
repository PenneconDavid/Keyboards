"""Render the HPD-specific proposal after David's 2026-10-24 feedback."""
import copy
import json
from pathlib import Path
from PIL import Image, ImageDraw
import render_review as view

ROOT = Path(__file__).parent
OUT = ROOT / 'proposal-v3'
OUT.mkdir(exist_ok=True)
prior = json.loads((ROOT/'proposal-v2/mapping-proposal.json').read_text(encoding='utf-8'))
data = copy.deepcopy(view.DATA)
for i,layer in enumerate(prior['layers']):
    data['layout'][i] = layer['matrix']
data['layout'][7] = copy.deepcopy(data['layout'][3])

def key(l,r,c,code): data['layout'][l][r][c] = code
def thumbs(l,left,right):
    for c,code in zip([4,5,0,1],left): key(l,4,c,code)
    for c,code in zip([1,0,5,4],right): key(l,10,c,code)
def extras(l,left,right):
    for (r,c),code in zip([(4,2),(4,3),(10,3),(10,2)],left+right): key(l,r,c,code)

ARROWS=['KC_LEFT','KC_DOWN','KC_UP','KC_RIGHT']
extras(0,['MO(7)','MO(2)'],['KC_BTN1','KC_BTN2'])
thumbs(0,['KC_LGUI','KC_LALT','MO(1)','KC_SPACE'],ARROWS)
key(0,9,0,'KC_ENTER')
# Keep FN/SYM and the mouse extras available on the number/navigation layer.
extras(1,['KC_TRNS','KC_TRNS'],['KC_BTN1','KC_BTN2'])
# Function menu remains above gaming layers, so it works from every mode.
for r in range(12):
    for c in range(7):
        if data['layout'][0][r][c] != -1: key(7,r,c,'KC_TRNS')
for c in range(6): key(7,0,c,f'KC_F{c+1}'); key(7,6,c,f'KC_F{12-c}')
for c,code in enumerate(['KC_ESCAPE','KC_MPRV','TO(5)','TO(6)','KC_MNXT','KC_MPLY']): key(7,1,c,code)
for c,code in enumerate(['KC_LSHIFT','LGUI(KC_LEFT)','LGUI(KC_DOWN)','LGUI(KC_UP)','LGUI(KC_RIGHT)','LGUI(KC_D)']): key(7,2,c,code)
for c,code in enumerate(['KC_LCTRL','LCTL(KC_Z)','LCTL(KC_X)','LCTL(KC_C)','LCTL(KC_V)','TO(0)']): key(7,3,c,code)
# Right home row: pointer functions while FN is explicitly held.
for c,code in enumerate(['KC_CAPSLOCK','USER59','KC_BTN3','KC_BTN2','KC_BTN1','USER58']): key(7,8,c,code)
for c,code in enumerate(['KC_DELETE','KC_PGUP','KC_END','KC_UP','KC_HOME','KC_INSERT']): key(7,7,c,code)
for c,code in enumerate(['KC_ENTER','KC_PGDOWN','KC_RIGHT','KC_DOWN','KC_LEFT','KC_NUMLOCK']): key(7,9,c,code)
key(7,6,0,'KC_F12')  # Delete is FN + physical backslash, preserving all F keys.
key(7,5,6,'KC_MPLY')

data['layout'][5] = copy.deepcopy(data['layout'][0])
extras(5,['MO(7)','KC_M'],['KC_BTN1','KC_BTN2'])
thumbs(5,['KC_LALT','KC_LCTRL','KC_C','KC_SPACE'],ARROWS)
data['layout'][6] = copy.deepcopy(data['layout'][5])
for r,codes in [(1,['KC_TAB','KC_ESCAPE','KC_Q','KC_W','KC_E','KC_R']),
                (2,['KC_LSHIFT','KC_LSHIFT','KC_A','KC_S','KC_D','KC_F']),
                (3,['KC_LCTRL','KC_LCTRL','KC_Z','KC_X','KC_C','KC_V'])]:
    for c,code in enumerate(codes): key(6,r,c,code)
extras(6,['MO(7)','KC_T'],['KC_BTN1','KC_BTN2'])
thumbs(6,['KC_LALT','KC_B','KC_G','KC_SPACE'],ARROWS)
# Replace a duplicated ESDF Ctrl with map; keep Ctrl at physical Z (3,1).
key(6,3,0,'KC_M')
# Keep right-hand M as well; both game layers now have plain left-hand M.

# Layers 3 and 4 are unused. Auto mouse is proposed OFF, not encoded in .vil.
for l in [3,4]:
    for r in range(12):
        for c in range(7):
            if data['layout'][0][r][c] != -1: key(l,r,c,'KC_TRNS')
for l in range(16): data['encoder_layout'][l] = [['KC_VOLD','KC_VOLU']]

view.DATA = data
view.TITLES = ['EVERYDAY / TRACKBALL-FIRST','NUMBER + NAVIGATION','SYMBOLS','UNUSED','UNUSED','GAME / WASD','GAME / ESDF','FUNCTION + POINTER + MEDIA']
view.SUBS = [
 'Dedicated number row, modifiers and thumb arrows. Right extras: mouse clicks. Enter replaces right Shift.',
 'Hold NUM/NAV (left thumb). Right numpad; left arrows / editing. Dedicated right thumb arrows stay active.',
 'Hold SYM (left bottom extra). Paired symbols and punctuation. Thumb arrows and mouse clicks remain.',
 '', '',
 'WASD: M/map on left bottom extra; Tab stays in pinky column. Thumbs: Alt / Ctrl / C / Space.',
 'ESDF: M/map on far-left bottom key; Ctrl beside it. Physical E/S/D/F sends W/A/S/D.',
 'Hold FN (left bottom extra). W = WASD; E = ESDF; B = Base. Pointer modes require deliberate key holds.']
view.LABELS.update({'MO(1)':'Hold NUM','MO(2)':'Hold SYM','MO(7)':'Hold FN',
 'TO(6)':'GO > ESDF','KC_BTN1':'Left click','KC_BTN2':'Right click',
 'KC_P0':'Num 0','KC_P1':'Num 1','KC_P2':'Num 2','KC_P3':'Num 3','KC_P4':'Num 4',
 'KC_P5':'Num 5','KC_P6':'Num 6','KC_P7':'Num 7','KC_P8':'Num 8','KC_P9':'Num 9',
 'KC_PDOT':'Num .','KC_PENT':'Num Enter','KC_PPLUS':'Num +','KC_PMINS':'Num -',
 'KC_PSLASH':'Num /','KC_PASTERISK':'Num *','KC_CAPSLOCK':'Caps Lock','KC_NUMLOCK':'Num Lock',
 'LCTL(KC_Z)':'Undo','LCTL(KC_X)':'Cut','LCTL(KC_C)':'Copy','LCTL(KC_V)':'Paste',
 'LGUI(KC_LEFT)':'Snap left','LGUI(KC_RIGHT)':'Snap right','LGUI(KC_UP)':'Maximize',
 'LGUI(KC_DOWN)':'Restore','LGUI(KC_D)':'Desktop'})

active=[0,1,2,7,5,6]
images={}
for l in active:
    im=view.sheet(l)
    d=ImageDraw.Draw(im)
    d.rectangle((0,817,1700,879),fill=view.BG)
    d.text((45,824),'V3 APPROVED MAPPING / 2026-10-25 / dark mode default / volume in ALL modes / not hardware-tested or flashed',font=view.font(18),fill=view.MUTED)
    im.save(OUT/f'layer-{l:02d}.png')
    images[l]=im
for name,layers in [('everyday-and-gaming',[0,5,6]),('everyday-and-utilities',[0,1,2,7])]:
    im=Image.new('RGB',(1700,880*len(layers)),view.BG)
    for i,l in enumerate(layers): im.paste(images[l],(0,880*i))
    im.save(OUT/f'{name}.png')
overview=Image.new('RGB',(1700,1320),view.BG)
for i,l in enumerate(active): overview.paste(images[l].resize((850,440)),((i%2)*850,(i//2)*440))
overview.save(OUT/'all-layers.png')

# Check requirements against generated data, not merely the labels.
for l in active:
    assert data['encoder_layout'][l] == [['KC_VOLD','KC_VOLU']]
    assert all(data['layout'][l][10][c] in (expected,'KC_TRNS')
               for c,expected in zip([1,0,5,4],ARROWS))
    assert data['layout'][l][4][2] in ('MO(7)','KC_TRNS')
    assert data['layout'][l][10][3] in ('KC_BTN1','KC_TRNS')
    assert data['layout'][l][10][2] in ('KC_BTN2','KC_TRNS')
assert [data['layout'][6][r][c] for r,c in [(1,3),(2,2),(2,3),(2,4)]] == ['KC_W','KC_A','KC_S','KC_D']
assert data['layout'][5][4][3] == 'KC_M' and data['layout'][5][1][0] == 'KC_TAB'
assert data['layout'][6][3][0] == 'KC_M' and data['layout'][6][3][1] == 'KC_LCTRL'
assert all(data['layout'][l][9][4] == 'KC_M' for l in [5,6])
(OUT/'mapping-proposal.json').write_text(json.dumps({
 'status':'user-approved mapping; this JSON is not a Vial import', 'date':'2026-10-25',
 'confirmed_requirements':['primary trackball','dedicated arrows and Ctrl/Shift/Alt/Win',
 'numpad layer','browsing and writing first; Excel supported','volume encoder across modes'],
 'pending_validation':['actual device identity','current layout backup','physical reach and runtime behavior'],
 'proposed_settings':{'auto_mouse_enabled':False,'sticky_pointer_modes':False},
 'layers':[{'index':l,'name':view.TITLES[l],'matrix':data['layout'][l],
 'encoder':data['encoder_layout'][l]} for l in active]
},indent=2),encoding='utf-8')
print('V3: six layers rendered; arrows, volume, pointer buttons, FN reachability and ESDF movement checked.')
