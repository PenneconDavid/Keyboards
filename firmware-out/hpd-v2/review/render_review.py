"""Render the existing Vial draft; never modifies the layout or a device."""
import hashlib
import json
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

HERE = Path(__file__).resolve().parent
SOURCE = HERE.parent / 'hpd_v2_gaming.vil'
RAW = SOURCE.read_bytes()
DATA = json.loads(RAW)
W, H = 1700, 880
BG, INK, MUTED = '#111820', '#edf2f7', '#afbdca'
KEY_BG, PASS_BG, OFF_BG = '#24313f', '#1b2631', '#161f28'
LAYER_BG, MOUSE_BG, MOVE_BG = '#614821', '#214e50', '#254e74'
STROKE, MODULE_BG = '#516170', '#202d39'
FONT = 'C:/Windows/Fonts/segoeui.ttf'
BOLD = 'C:/Windows/Fonts/segoeuib.ttf'
def font(n, bold=False):
    return ImageFont.truetype(BOLD if bold else FONT, n)

LABELS = {'KC_TRNS':'PASS', 'KC_NO':'OFF', 'KC_ESCAPE':'Esc', 'KC_GRAVE':'`',
 'KC_BSPACE':'Backspace', 'KC_TAB':'Tab', 'KC_LSHIFT':'Shift', 'KC_RSHIFT':'Shift',
 'KC_LCTRL':'Ctrl', 'KC_RCTRL':'Ctrl', 'KC_LALT':'Alt', 'KC_RALT':'Alt',
 'KC_RGUI':'Win', 'KC_LGUI':'Win', 'KC_SPACE':'Space', 'KC_ENTER':'Enter',
 'KC_DELETE':'Delete', 'KC_PGDOWN':'PgDn', 'KC_PGUP':'PgUp', 'KC_BSLASH':'\\',
 'KC_QUOTE':"'", 'KC_SCOLON':';', 'KC_SLASH':'/', 'KC_DOT':'.', 'KC_COMMA':',',
 'KC_LBRACKET':'[', 'KC_RBRACKET':']', 'KC_MUTE':'Mute', 'KC_VOLU':'Vol +',
 'KC_VOLD':'Vol -', 'KC_MNXT':'Next', 'KC_MPRV':'Previous', 'KC_MPLY':'Play/pause',
 'KC_INSERT':'Insert', 'KC_PSCREEN':'PrtSc', 'KC_AT':'@', 'KC_DLR':'$',
 'KC_HASH':'#', 'KC_LT':'<', 'KC_EQUAL':'=', 'KC_GT':'>', 'KC_CIRC':'^',
 'KC_UNDS':'_', 'KC_EXLM':'!', 'KC_RCBR':'}', 'KC_LCBR':'{', 'KC_PERC':'%',
 'KC_LPRN':'(', 'KC_RPRN':')', 'KC_MINUS':'-', 'KC_PLUS':'+', 'KC_ASTR':'*',
 'KC_COLN':':', 'KC_AMPR':'&', 'KC_TILD':'~', 'KC_PIPE':'|',
 'KC_BTN1':'Fire / LMB', 'KC_BTN2':'ADS / RMB', 'KC_BTN3':'Middle btn',
 'USER58':'Sniper', 'USER59':'Scroll mode', 'USER60':'Text mode',
 'MO(1)':'Hold Lower', 'MO(2)':'Hold Raise', 'TO(0)':'EXIT > Base',
 'TO(5)':'GO > GAME', 'TO(6)':'GO > ARC', 'TO(7)':'GO > ESDF',
 'KC_WH_D':'Wheel down', 'KC_WH_U':'Wheel up',
 'LCTL(KC_LEFT)':'Ctrl + Left', 'LCTL(KC_RIGHT)':'Ctrl + Right'}
def label(code):
    return LABELS.get(code, code.removeprefix('KC_').title() if len(code)>4 else code.removeprefix('KC_'))

TITLES = ['BASE', 'LOWER', 'RAISE', 'ADJUST', 'MOUSE', 'GAME / WASD', 'ARC RAIDERS', 'GAME / ESDF']
SUBS = [
 'Stock-style fallback in the draft. Your actual typing layout has NOT been exported or merged.',
 'Hold the Base left-thumb Lower key. Three right-pinky keys select the gaming layers.',
 'Hold the Base right-thumb Raise key. Symbols remain as saved in the draft.',
 'All keys pass through. The draft does not establish a way to enter this layer.',
 'Intended auto-mouse layer 4. Left keys pass through; activation/settings need board verification.',
 'Enter: Lower + physical right \\ key. Plain WASD; no layer holds or Windows keys.',
 'Enter: Lower + physical right apostrophe key. Physical R sends M; reload stays on a thumb.',
 'Enter: Lower + physical right Shift key. Hand on E/S/D/F sends W/A/S/D.']

def sheet(layer):
    im = Image.new('RGB', (W,H), BG)
    d = ImageDraw.Draw(im)
    d.text((45,25), f'{layer:02d}   {TITLES[layer]}', font=font(32,True), fill=INK)
    d.text((45,77), SUBS[layer], font=font(21), fill=MUTED)
    d.text((45,116), 'Top view / schematic spacing. Large text = output; small r,c = Vial matrix address.', font=font(18), fill=MUTED)
    d.text((65,158), 'LEFT  /  pinky to inner', font=font(20,True), fill=INK)
    d.text((1010,158), 'RIGHT  /  inner to pinky', font=font(20,True), fill=INK)
    count = 0
    def key(x,y,r,c):
        nonlocal count
        code = DATA['layout'][layer][r][c]
        assert code != -1
        count += 1
        text = label(code)
        fill = KEY_BG
        if code == 'KC_TRNS': fill = PASS_BG
        elif code == 'KC_NO': fill = OFF_BG
        elif code.startswith(('TO(', 'MO(')): fill = LAYER_BG
        elif code in ('KC_BTN1','KC_BTN2','KC_BTN3','USER58','USER59','USER60'): fill = MOUSE_BG
        elif layer >= 5 and code in ('KC_W','KC_A','KC_S','KC_D'): fill = MOVE_BG
        d.rounded_rectangle((x,y,x+91,y+65), radius=8, fill=fill, outline=STROKE, width=1)
        n = 20
        while d.textlength(text,font=font(n,True)) > 83: n -= 1
        d.text((x+45,y+22),text,anchor='mm',font=font(n,True),fill=INK)
        d.text((x+45,y+50),f'{r},{c}',anchor='mm',font=font(13),fill=MUTED)
    # Main blocks: column direction mirrors the right half physically.
    for rr in range(4):
        for cc in range(6):
            key(65+cc*101,202+rr*76,rr,cc)
            key(1030+cc*101,202+rr*76,rr+6,5-cc)
    d.text((260,519),'Bottom extras',font=font(17),fill=MUTED)
    key(267,547,4,2); key(368,547,4,3)
    d.text((1230,519),'Bottom extras',font=font(17),fill=MUTED)
    key(1232,547,10,3); key(1333,547,10,2)
    # Thumb arcs are drawn in outside-to-inside order on left; reversed on right.
    d.text((300,645),'Thumb arc: outer > inner',font=font(17),fill=MUTED)
    for i,c in enumerate([4,5,0,1]): key(300+101*i,680,4,c)
    d.text((1000,645),'Thumb arc: inner > outer',font=font(17),fill=MUTED)
    for i,c in enumerate([1,0,5,4]): key(1000+101*i,680,10,c)
    d.ellipse((735,220,935,420),fill=MODULE_BG,outline=STROKE,width=2)
    d.text((835,290),'TRACKBALL',anchor='mm',font=font(20,True),fill=INK)
    d.text((835,323),'right inner module',anchor='mm',font=font(16),fill=MUTED)
    enc = DATA['encoder_layout'][layer][0]
    d.text((735,459),'LEFT ENCODER',font=font(18,True),fill=INK)
    d.text((735,490),'CCW: '+label(enc[0]),font=font(18),fill=INK)
    d.text((735,518),'CW:   '+label(enc[1]),font=font(18),fill=INK)
    key(765,558,5,6)
    assert count == 61, count
    d.line((45,774,1655,774),fill=STROKE,width=1)
    d.text((45,789),'PASS = use active lower layer  |  OFF = no action  |  Blue = movement  |  Teal = mouse  |  Amber = layer access',font=font(19),fill=INK)
    d.text((45,824),'DRAFT / not hardware-tested   •   Source: hpd_v2_gaming.vil   •   Review rendered 2026-10-23',font=font(18),fill=MUTED)
    return im

if __name__ == '__main__':
    images = []
    for layer in range(8):
        im = sheet(layer)
        im.save(HERE / f'layer-{layer:02d}.png')
        images.append(im)
    overview = Image.new('RGB',(W,H*3),BG)
    for i,layer in enumerate([5,6,7]): overview.paste(images[layer],(0,i*H))
    overview.save(HERE/'gaming-overview.png')
    assert len(DATA['layout']) == 16
    print('Rendered 8 layer images and gaming overview; 61 physical switches per layer including encoder click.')
    print('Source SHA256:', hashlib.sha256(RAW).hexdigest())
