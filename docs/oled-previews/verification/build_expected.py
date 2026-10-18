from pathlib import Path
import re
base=Path(__file__).resolve().parent
source=(base.parent/'sources/luna.c').read_text()
frames=[]
for pose in ('sit','walk','run','bark','sneak'):
 start=re.search(r'\b'+pose+r'\s*\[[^;=]+?=\s*\{',source).end()-1
 depth=0
 for end in range(start,len(source)):
  depth+=(source[end]=='{')-(source[end]=='}')
  if not depth:break
 body=re.sub(r'/\*.*?\*/|//[^\n]*','',source[start:end+1],flags=re.S)
 data=bytes(int(v,16) for v in re.findall(r'0x([0-9a-fA-F]+)',body));assert len(data)==192
 frames.extend((data[:96],data[96:]))
header='/* Original Luna sprite test fixture. Copyright2021 HellSingCoder, GPL-2.0-or-later. */\n'
header+='static const uint8_t expected_luna_frames[10][96] = {\n'+''.join('{'+','.join(str(v) for v in f)+'},\n' for f in frames)+'};\n'
(base/'luna_expected.h').write_text(header)
from PIL import Image
cards=[]
for i in range(7):
 im=Image.open(next((base.parent/'final').glob(f'left-{i}-*.png')))
 cards.append(bytes(sum((im.getpixel((x,y+b))!=0)<<b for b in range(8)) for y in range(0,128,8) for x in range(32)))
with (base/'luna_expected.h').open('a') as f:
 f.write('static const uint8_t expected_cards[7][512] = {\n'+''.join('{'+','.join(str(v) for v in c)+'},\n' for c in cards)+'};\n')
