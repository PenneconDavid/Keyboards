"""Create review-only v2 proposal data and diagrams. No loadable Vial file."""
import copy
import json
from pathlib import Path
from PIL import Image, ImageDraw
import render_review as renderer

OUT = Path(__file__).parent / 'proposal-v2'
OUT.mkdir(exist_ok=True)
proposal = copy.deepcopy(renderer.DATA)

def setkey(layer,r,c,code): proposal['layout'][layer][r][c] = code
def row(layer,r,codes):
    assert len(codes) == 6
    for c,code in enumerate(codes): setkey(layer,r,c,code)
def extras(layer,left,right):
    for (r,c),code in zip([(4,2),(4,3),(10,3),(10,2)],left+right): setkey(layer,r,c,code)
def thumbs(layer,left,right):
    for c,code in zip([4,5,0,1],left): setkey(layer,4,c,code)
    for c,code in zip([1,0,5,4],right): setkey(layer,10,c,code)

# V2 uses the full alphabet, number row, dedicated modifiers and layer keys.
setkey(0,0,0,'KC_ESCAPE')
setkey(0,1,0,'KC_TAB')
setkey(0,2,0,'KC_LSHIFT')
setkey(0,3,0,'KC_LCTRL')
extras(0,['KC_GRAVE','MO(2)'],['KC_BTN1','KC_BTN2'])
thumbs(0,['KC_LGUI','KC_LALT','MO(1)','KC_SPACE'],
       ['KC_ENTER','KC_BSPACE','KC_DELETE','MO(3)'])

# Start utility layers as pass-throughs rather than inheriting the old proposal.
for layer in [1,2,3,4]:
    for r in range(12):
        for c in range(7):
            if proposal['layout'][0][r][c] != -1: setkey(layer,r,c,'KC_TRNS')

# NAV: left inverted-T, right numeric block. Extras do document navigation.
row(1,1,['KC_TAB','KC_HOME','KC_UP','KC_END','KC_PGUP','KC_INSERT'])
row(1,2,['KC_LSHIFT','KC_LEFT','KC_DOWN','KC_RIGHT','KC_PGDOWN','KC_DELETE'])
row(1,3,['KC_LCTRL','LCTL(KC_Z)','LCTL(KC_X)','LCTL(KC_C)','LCTL(KC_V)','KC_ENTER'])
row(1,7,['KC_PSLASH','KC_P9','KC_P8','KC_P7','KC_PASTERISK','KC_NO'])
row(1,8,['KC_PMINS','KC_P6','KC_P5','KC_P4','KC_PPLUS','KC_NO'])
row(1,9,['KC_PENT','KC_P3','KC_P2','KC_P1','KC_PDOT','KC_P0'])
extras(1,['LCTL(KC_HOME)','LCTL(KC_END)'],['KC_HOME','KC_END'])

# SYMBOLS: symmetric pairs and the missing everyday punctuation.
row(2,1,['KC_TAB','KC_EXLM','KC_AT','KC_HASH','KC_DLR','KC_PERC'])
row(2,2,['KC_LSHIFT','KC_BSLASH','KC_LPRN','KC_RPRN','KC_MINUS','KC_PLUS'])
row(2,3,['KC_LCTRL','KC_GRAVE','KC_LBRACKET','KC_RBRACKET','KC_EQUAL','KC_UNDS'])
row(2,7,['KC_BSLASH','KC_RCBR','KC_LCBR','KC_RBRACKET','KC_LBRACKET','KC_CIRC'])
row(2,8,['KC_QUOTE','KC_SCOLON','KC_RPRN','KC_LPRN','KC_AMPR','KC_ASTR'])
row(2,9,['KC_PIPE','KC_TILD','KC_GT','KC_LT','KC_COLN','KC_SLASH'])

# FN: function row, media, desktop utilities, deliberately held game selection.
row(3,0,[f'KC_F{i}' for i in range(1,7)])
row(3,6,[f'KC_F{i}' for i in range(12,6,-1)])
row(3,1,['KC_ESCAPE','KC_MPRV','TO(5)','TO(6)','KC_MNXT','KC_MPLY'])
row(3,2,['KC_LSHIFT','LGUI(KC_LEFT)','LGUI(KC_DOWN)','LGUI(KC_UP)','LGUI(KC_RIGHT)','LGUI(KC_D)'])
row(3,3,['KC_LCTRL','LCTL(KC_Z)','LCTL(KC_X)','LCTL(KC_C)','LCTL(KC_V)','TO(0)'])
row(3,7,['KC_PSCREEN','KC_PGUP','KC_HOME','KC_UP','KC_END','KC_INSERT'])
row(3,8,['KC_CAPSLOCK','KC_PGDOWN','KC_LEFT','KC_DOWN','KC_RIGHT','KC_DELETE'])
setkey(3,9,5,'KC_NUMLOCK')

# POINTER: typing letters only change during mouse mode; left half stays intact.
row(4,8,['KC_TRNS','USER59','KC_BTN3','KC_BTN2','KC_BTN1','USER58'])
extras(4,['KC_TRNS','KC_TRNS'],['KC_BTN1','KC_BTN2'])

# GAME: a complete letter block for chat, no copied Corne-specific reload/heal row.
proposal['layout'][5] = copy.deepcopy(proposal['layout'][0])
extras(5,['KC_M','KC_TAB'],['USER58','TO(0)'])
thumbs(5,['KC_LALT','KC_C','KC_V','KC_SPACE'],['KC_BTN1','KC_BTN2','KC_ENTER','KC_DELETE'])
setkey(5,5,6,'KC_MUTE')
proposal['layout'][6] = copy.deepcopy(proposal['layout'][5])
# Shift the left-hand letter block one column inward for physical ESDF => WASD.
row(6,1,['KC_TAB','KC_ESCAPE','KC_Q','KC_W','KC_E','KC_R'])
row(6,2,['KC_LSHIFT','KC_LSHIFT','KC_A','KC_S','KC_D','KC_F'])
row(6,3,['KC_LCTRL','KC_LCTRL','KC_Z','KC_X','KC_C','KC_V'])
extras(6,['KC_T','KC_G'],['USER58','TO(0)'])
thumbs(6,['KC_LALT','KC_B','KC_M','KC_SPACE'],['KC_BTN1','KC_BTN2','KC_ENTER','KC_DELETE'])

proposal['encoder_layout'][1] = [['KC_WH_D','KC_WH_U']]
proposal['encoder_layout'][2] = [['KC_VOLD','KC_VOLU']]
proposal['encoder_layout'][3] = [['KC_MPRV','KC_MNXT']]
proposal['encoder_layout'][4] = [['KC_WH_D','KC_WH_U']]
for l in [5,6]: proposal['encoder_layout'][l] = [['KC_WH_D','KC_WH_U']]

renderer.DATA = proposal
renderer.TITLES = ['EVERYDAY / QWERTY','NAVIGATION + NUMPAD','SYMBOLS','FUNCTION + MEDIA','POINTER / AUTO MOUSE','GAME / WASD','GAME / ESDF']
renderer.SUBS = [
 'First-pass design: dedicated number row + modifiers; thumb editing; permanent mouse clicks on right extras.',
 'Hold NAV (left thumb). Left inverted-T arrows; right numpad uses Num Lock; Ctrl/Shift stay available.',
 'Hold SYM (left bottom extra). Paired brackets, operators and punctuation; ordinary Space and Enter remain.',
 'Hold FN (outer right thumb). FN + W enters WASD; FN + E enters ESDF; top row is F1-F12.',
 'Candidate mouse layer 4. Preserve the left hand; right home row provides buttons and pointer modes.',
 'Normal QWERTY letter block retained. Extra left keys: map / inventory; thumbs: Alt / C / V / Space.',
 'Physical E/S/D/F sends W/A/S/D. Other left letters shift; return to Base for normal chat typing.']
renderer.LABELS.update({'MO(1)':'Hold NAV','MO(2)':'Hold SYM','MO(3)':'Hold FN',
 'TO(6)':'GO > ESDF','KC_BTN1':'Left click','KC_BTN2':'Right click',
 'KC_P0':'Num 0','KC_P1':'Num 1','KC_P2':'Num 2','KC_P3':'Num 3','KC_P4':'Num 4',
 'KC_P5':'Num 5','KC_P6':'Num 6','KC_P7':'Num 7','KC_P8':'Num 8','KC_P9':'Num 9',
 'KC_PDOT':'Num .','KC_PENT':'Num Enter','KC_PPLUS':'Num +','KC_PMINS':'Num -',
 'KC_PSLASH':'Num /','KC_PASTERISK':'Num *','KC_CAPSLOCK':'Caps Lock','KC_NUMLOCK':'Num Lock',
 'LCTL(KC_HOME)':'Ctrl + Home','LCTL(KC_END)':'Ctrl + End',
 'LCTL(KC_Z)':'Undo','LCTL(KC_X)':'Cut','LCTL(KC_C)':'Copy','LCTL(KC_V)':'Paste',
 'LGUI(KC_LEFT)':'Snap left','LGUI(KC_RIGHT)':'Snap right','LGUI(KC_UP)':'Maximize',
 'LGUI(KC_DOWN)':'Restore','LGUI(KC_D)':'Desktop'})

images=[]
for layer in range(7):
    im=renderer.sheet(layer)
    d=ImageDraw.Draw(im)
    d.rectangle((0,817,1700,879),fill=renderer.BG)
    d.text((45,824),'V2 DISCUSSION DRAFT / assumptions pending / 2026-10-24 / diagram only; no flashable or loadable output',font=renderer.font(18),fill=renderer.MUTED)
    im.save(OUT/f'layer-{layer:02d}.png')
    images.append(im)
overview=Image.new('RGB',(1700,880*3),renderer.BG)
for i,layer in enumerate([0,5,6]): overview.paste(images[layer],(0,i*880))
overview.save(OUT/'everyday-and-gaming.png')
(OUT/'mapping-proposal.json').write_text(json.dumps({
 'status':'discussion draft; not a Vial import file', 'date':'2026-10-24',
 'assumptions':['Windows, US QWERTY','trackball main pointer; separate mouse remains possible',
 'dedicated modifiers and layer keys','ESDF physical posture sends WASD',
 'no forced Corne parity; no dedicated Arc layer until games and bindings are known'],
 'layers':[{'name':renderer.TITLES[i],'matrix':proposal['layout'][i],
 'encoder':proposal['encoder_layout'][i]} for i in range(7)]
},indent=2),encoding='utf-8')
print('Rendered seven V2 discussion layers and an Everyday/WASD/ESDF comparison. Original .vil untouched.')
