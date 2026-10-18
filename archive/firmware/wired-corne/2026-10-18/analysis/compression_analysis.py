from pathlib import Path
import re
from PIL import Image
base=Path('docs/oled-previews/final')
def pack(im):
 return bytes(sum((im.getpixel((x,y+b))!=0)<<b for b in range(8)) for y in range(0,128,8) for x in range(32))
cards=[pack(Image.open(next(base.glob(f'left-{i}-*.png')))) for i in range(7)]
s=Path('keyboards/crkbd/keymaps/keebv2/oled_assets.h').read_text().split('luna_frames',1)[1]
nums=bytes(int(x,16) for x in re.findall('0x([0-9a-fA-F]+)',s))
sprites=[nums[i:i+96] for i in range(0,len(nums),96)]
def zrle(data):
 out=[];i=0
 while i<len(data):
  zero=data[i]==0;j=i+1
  while j<len(data) and (data[j]==0)==zero and j-i<127:j+=1
  out.append(j-i if zero else 128+j-i)
  if not zero:out.extend(data[i:j])
  i=j
 return bytes(out)
def rle(data):
 # optimal PackBits repeating-byte packets and literal packets, max128
 n=len(data);dp=[0]*(n+1)
 for i in range(n-1,-1,-1):
  dp[i]=min(1+k+dp[i+k] for k in range(1,min(128,n-i)+1))
  for k in range(2,min(128,n-i)+1):
   if data[i+k-1]!=data[i]:break
   dp[i]=min(dp[i],2+dp[i+k])
 return dp[0]
for name,data in [('cards',cards),('luna',sprites)]:
 print(name, 'raw',sum(map(len,data)), 'zeroRLE',sum(len(zrle(x)) for x in data),'packbits',sum(rle(x) for x in data))
 print('sizes', [len(zrle(x)) for x in data])
# Store card union shared indexed unique columns? 128-row 16-byte columns; 7*32=224 vectors
cols=[b''.join(c[p*32+x:p*32+x+1] for p in range(16)) for c in cards for x in range(32)]
u=list(dict.fromkeys(cols));print('column dict',len(u),len(u)*16+224,'zero compressed unique',sum(len(zrle(x)) for x in u)+224)
# Shared fixed element raster spans rectangles all text/icon draws converted runs
# dedup 32byte rows (pages) zero RLE each shared tile and offsets
for size in [8,16,32,64]:
 chunks=[c[i:i+size] for c in cards for i in range(0,512,size)];uniq=list(dict.fromkeys(chunks))
 print('tile',size,'unique',len(uniq),'raw',len(uniq)*size+len(chunks),'compressed',sum(len(zrle(x)) for x in uniq)+2*len(uniq)+len(chunks))
