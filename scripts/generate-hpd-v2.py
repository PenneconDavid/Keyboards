"""Generate the approved HPD PC/TV keymap and Vial import, without device access."""
import argparse
import copy
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PIN = '98bf33054c73619b590505853d8d8f3e0557f41e'
NAME = 'pc_tv_v3'
parser = argparse.ArgumentParser()
parser.add_argument('--upstream', type=Path, default=ROOT/'.build/hpd-v2/vial-qmk')
args = parser.parse_args()
upstream = args.upstream.resolve()
revision = subprocess.check_output(['git','-C',str(upstream),'rev-parse','HEAD'],text=True).strip()
if revision != PIN:
    raise SystemExit(f'Expected upstream {PIN}; got {revision}')

source = ROOT/'firmware-out/hpd-v2/review/proposal-v3/mapping-proposal.json'
approved = json.loads(source.read_text(encoding='utf-8'))
original = json.loads((ROOT/'firmware-out/hpd-v2/hpd_v2_gaming.vil').read_text(encoding='utf-8'))
keydir = ROOT/'keyboards/ergohaven/hpd/keymaps'/NAME
out = ROOT/'firmware-out/hpd-v2/pc-tv-v3'
keydir.mkdir(parents=True,exist_ok=True)
out.mkdir(parents=True,exist_ok=True)

# These diagram labels were not actual keycodes. Preserve meaning using Vial names.
vial_aliases = {
    'KC_PSLASH':'KC_KP_SLASH', 'KC_PASTERISK':'KC_KP_ASTERISK',
    'KC_PPLUS':'KC_KP_PLUS', 'KC_PMINS':'KC_KP_MINUS',
}
c_aliases = {
    'KC_ESCAPE':'KC_ESC','KC_BSPACE':'KC_BSPC','KC_LSHIFT':'KC_LSFT',
    'KC_RSHIFT':'KC_RSFT','KC_LCTRL':'KC_LCTL','KC_RCTRL':'KC_RCTL',
    'KC_ENTER':'KC_ENT','KC_DELETE':'KC_DEL','KC_GRAVE':'KC_GRV',
    'KC_BSLASH':'KC_BSLS','KC_QUOTE':'KC_QUOT','KC_SCOLON':'KC_SCLN',
    'KC_SLASH':'KC_SLSH','KC_COMMA':'KC_COMM','KC_LBRACKET':'KC_LBRC',
    'KC_RBRACKET':'KC_RBRC','KC_PSCREEN':'KC_PSCR','KC_PGDOWN':'KC_PGDN',
    'KC_INSERT':'KC_INS','KC_MINUS':'KC_MINS','KC_EQUAL':'KC_EQL',
    'KC_CAPSLOCK':'KC_CAPS','KC_NUMLOCK':'KC_NUM',
    'KC_KP_SLASH':'KC_PSLS','KC_KP_ASTERISK':'KC_PAST',
    'KC_KP_PLUS':'KC_PPLS','KC_KP_MINUS':'KC_PMNS',
    'USER58':'EH_SNP','USER59':'EH_SCR',
}
def substitute(code,aliases):
    return re.sub(r'\b[A-Z][A-Z0-9_]*\b',lambda m:aliases.get(m[0],m[0]),code)

mask = original['layout'][0]
layers = [[[('KC_TRNS' if mask[r][c] != -1 else -1) for c in range(7)] for r in range(12)] for _ in range(16)]
for layer in approved['layers']:
    layers[layer['index']] = copy.deepcopy(layer['matrix'])
for layer in layers:
    for row in layer:
        for c,code in enumerate(row):
            if code != -1: row[c] = substitute(code,vial_aliases)

def resolve(stack,r,c):
    for l in sorted(set(stack)|{0},reverse=True):
        if layers[l][r][c] != 'KC_TRNS': return layers[l][r][c]
    return 'KC_NO'

for l in [0,1,2,5,6,7]:
    assert resolve([l],4,2) == 'MO(7)'
    assert [resolve([l],10,c) for c in [1,0,5,4]] == ['KC_LEFT','KC_DOWN','KC_UP','KC_RIGHT']
    assert [resolve([l],10,c) for c in [3,2]] == ['KC_BTN1','KC_BTN2']
assert layers[5][4][3] == layers[6][3][0] == 'KC_M'
assert layers[5][1][0] == layers[6][1][0] == 'KC_TAB'
assert layers[6][3][1] == 'KC_LCTRL'
assert [layers[6][r][c] for r,c in [(1,3),(2,2),(2,3),(2,4)]] == ['KC_W','KC_A','KC_S','KC_D']
assert [resolve([5,7],1,c) for c in [2,3]] == ['TO(5)','TO(6)']
assert resolve([6,7],3,5) == 'TO(0)'

bundle = copy.deepcopy(original)
bundle['layout'] = layers
bundle['encoder_layout'] = [[['KC_VOLD','KC_VOLU']] for _ in range(16)]
vil = out/'hpd_v2_pc_tv_v3.vil'
vil.write_text(json.dumps(bundle,indent=2)+'\n',encoding='utf-8')

header = ('// Generated from the user-approved 2026-10-25 v3 proposal.\n'
          '// Regenerate with scripts/generate-hpd-v2.py; do not edit bindings here.\n'
          '#include QMK_KEYBOARD_H\n#include "ergohaven.h"\n#include "src/eh_pointing.h"\n\n'
          '_Static_assert(EH_SNP == QK_KB + 58, "Vial USER58 must be Sniper");\n'
          '_Static_assert(EH_SCR == QK_KB + 59, "Vial USER59 must be Scroll");\n\n'
          'const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {\n')
parts=[header]
for l,layer in enumerate(layers):
    parts.append(f'    [{l}] = {{\n')
    for row in layer:
        parts.append('        { '+', '.join(substitute('KC_NO' if c == -1 else c,c_aliases) for c in row)+' },\n')
    parts.append('    },\n')
parts.append('};\n\n#ifdef ENCODER_MAP_ENABLE\nconst uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {\n')
for l in range(16): parts.append(f'    [{l}] = {{ ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }},\n')
parts.append('};\n#endif\n\n')
parts.append('''// Policy: never turn writing keys into mouse keys automatically; modes are held.
// The vendor setters only write EEPROM when the value changes. Other pointer
// preferences (DPI, orientation and sensitivities) remain untouched.
void keyboard_post_init_user(void) {
    if (get_split_pointing_auto_mouse_enable()) {
        set_split_pointing_auto_mouse_enable(false);
    }
    if (get_sticky_mode()) {
        set_sticky_mode(false);
    }
    set_pointing_mode(POINTING_MODE_NORMAL);
}
''')
(keydir/'keymap.c').write_text(''.join(parts),encoding='utf-8')

vendor = upstream/'keyboards/ergohaven/hpd/keymaps/v2_enc_ball'
for name in ['config.h','rules.mk','halconf.h','mcuconf.h','vial.json']:
    shutil.copyfile(vendor/name,keydir/name)

# Install into the isolated checkout only, keeping the canonical sources here.
build_keydir = upstream/'keyboards/ergohaven/hpd/keymaps'/NAME
build_keydir.mkdir(exist_ok=True)
for item in keydir.iterdir():
    if item.is_file() and item.name != 'README.md': shutil.copyfile(item,build_keydir/item.name)

def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
manifest = {
    'date':'2026-10-25','status':'sources generated; compilation pending',
    'target':'ergohaven/hpd/rev2','keymap':NAME,'upstream_revision':PIN,
    'approved_proposal_sha256':sha(source),'vial_uid':bundle['uid'],
    'matrix':{'layers':16,'rows':12,'columns':7},
    'policy':{'auto_mouse_enabled_at_boot':False,'sticky_modes_at_boot':False},
    'vial_sha256':sha(vil),
    'sources':{p.name:sha(p) for p in keydir.iterdir() if p.is_file() and p.name != 'README.md'},
    'validation':['layer resolution','left-hand map and inventory','dedicated arrows',
                  'clicks across layers','ESDF movement','FN game selectors and exit'],
    'hardware_tested':False,'flashed':False,
}
(out/'source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
print(f'Generated {keydir.relative_to(ROOT)} and {vil.relative_to(ROOT)}')
print('Validated approved binding resolution; no connected-device writes performed.')
