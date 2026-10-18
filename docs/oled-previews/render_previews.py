"""Render review-only 1-bit OLED concepts. Does not touch firmware sources."""
from pathlib import Path
import re, json, hashlib
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parent
QMK = Path.home() / 'qmk_firmware/keyboards/crkbd/lib'
def clean(s):
    return re.sub(r'/\*.*?\*/|//[^\n]*', '', s, flags=re.S)
def numbers(s):
    return [int(x,16) for x in re.findall(r'0x([0-9a-fA-F]+)', clean(s))]
def array(s, name):
    start = re.search(r'\b'+name+r'\s*\[[^;=]+?=\s*\{',s).end()-1
    depth=0
    for i in range(start,len(s)):
        depth += (s[i]=='{') - (s[i]=='}')
        if depth==0: return numbers(s[start:i+1])
def raw(data,w,h):
    im=Image.new('1',(w,h))
    for i,v in enumerate(data):
        for b in range(8):
            x,y=i%w,(i//w)*8+b
            if y<h and v & (1<<b): im.putpixel((x,y),1)
    return im
fontbytes=numbers((QMK/'glcdfont.c').read_text().split('= {',1)[1])
def text(im,xy,s):
    x,y=xy
    for ch in s:
        glyph=raw(fontbytes[ord(ch)*6:ord(ch)*6+6],6,8)
        im.paste(glyph,(x,y)); x+=6
def centered(im,y,s): text(im,((32-len(s)*6)//2,y),s)
def card(label='BASE',kind='keys'):
    im=Image.new('1',(32,128)); d=ImageDraw.Draw(im)
    d.line((2,8,29,8),fill=1); centered(im,18,label)
    if kind=='keys':
        for row in range(3):
            for col in range(3): d.rectangle((4+col*8,48+row*9,9+col*8,54+row*9),outline=1)
        d.rectangle((10,80,22,85),outline=1)
    elif kind=='game':
        d.rounded_rectangle((2,53,29,75),radius=5,outline=1)
        d.line((7,59,7,69),fill=1); d.line((3,64,11,64),fill=1)
        d.rectangle((21,59,22,60),fill=1); d.rectangle((25,65,26,66),fill=1)
    else:
        centered(im,52,'!@#$'); centered(im,66,'1234'); centered(im,80,'<>[]')
    d.line((2,112,29,112),fill=1)
    return im
def monogram():
    im=Image.new('1',(32,128)); d=ImageDraw.Draw(im)
    d.line((3,12,28,12),fill=1)
    # Deliberate large D mark, kept on the 32-pixel grid.
    d.line([(8,37),(19,37),(25,43),(25,75),(19,81),(8,81),(8,37)],fill=1,width=2)
    d.line((13,43,13,75),fill=1)
    centered(im,94,'DAVID'); d.line((3,115,28,115),fill=1)
    return im
luna=(ROOT/'sources/luna.c').read_text()
dogs={name:[raw(array(luna,name)[i*96:(i+1)*96],32,24) for i in range(2)] for name in ['sit','walk','run','bark','sneak']}
def pet(state,i):
    im=Image.new('1',(32,128)); im.paste(dogs[state][i],(0,96)); return im
bongo=(ROOT/'sources/bongo.c').read_text()
bongo_data={n:array(bongo,n) for n in ['idle','prep','tap']}
# Each initializer has 512 artwork bytes; C pads each declared 636-byte slot.
catframes=[raw(bongo_data['tap'][i*512:(i+1)*512],128,32) for i in range(2)]
logo_codes=numbers((QMK/'logo_reader.c').read_text())
logo=Image.new('1',(128,32))
for i,c in enumerate(logo_codes):
    if c==0: break
    logo.paste(raw(fontbytes[c*6:c*6+6],6,8),((i%21)*6,4+(i//21)*8))
status=Image.new('1',(32,128))
for row,s in enumerate(['LAYER','','BASE','','MODS','S C A','G    ','','CAPS','OFF','','RGB','CYCLE']): text(status,(1,row*8),s)
def ui_font(n,bold=False):
    return ImageFont.truetype('C:/Windows/Fonts/'+('segoeuib.ttf' if bold else 'segoeui.ttf'),n)
BG='#10161e'; FG='#f0f4f8'; MUTED='#a6b5c7'; ACC='#85d8ef'
def pixels(im,scale=3):
    return im.convert('RGB').resize((im.width*scale,im.height*scale),Image.Resampling.NEAREST)
def screen(canvas,im,x,y,scale=3):
    d=ImageDraw.Draw(canvas); w,h=im.width*scale,im.height*scale
    d.rounded_rectangle((x-9,y-9,x+w+8,y+h+8),radius=8,fill='#030405',outline='#354354',width=1)
    canvas.paste(pixels(im,scale),(x,y))
def panel(letter,title,sub,im,lines,inset=None):
    out=Image.new('RGB',(510,690),BG); d=ImageDraw.Draw(out)
    d.text((24,18),letter,fill=ACC,font=ui_font(20,True))
    d.text((24,50),title,fill=FG,font=ui_font(25,True))
    d.text((24,88),sub,fill=MUTED,font=ui_font(15))
    screen(out,im,62,139)
    if inset is not None:
        d.text((195,150),'Upright artwork',fill=MUTED,font=ui_font(14))
        screen(out,inset,195,184,2)
    for j,line in enumerate(lines): d.text((24,558+j*24),line,fill=MUTED,font=ui_font(15))
    d.text((24,659),'1-bit pixels | enlarged 3x | review only',fill='#6e8298',font=ui_font(13))
    return out
panels={}
panels['A']=panel('A','Bongo Cat','Referenced source; adaptation required',catframes[0].transpose(Image.Transpose.ROTATE_90),[
    'Art is 128 x 32, shown sideways in the tall panel.',
    'Source reserves 8 x 636 = 5,088 bytes for frames.',
    'Needs trimming/compression to fit the budget.'],catframes[0])
panels['B']=panel('B','Luna keyboard pet','Actual Luna sprites; sample placement',pet('sit',0),[
    'Portrait-native; actual sprite is 32 x 24 storage.',
    'Sit / walk / run / bark / sneak; two frames each.',
    'Preview uses Luna. Felix is a separate art variant.'])
# Show actual other poses in the open space beside the portrait panel.
d=ImageDraw.Draw(panels['B'])
for j,state in enumerate(['walk','run','bark','sneak']):
    y=150+j*87; d.text((210,y),state.upper(),font=ui_font(13),fill=MUTED)
    panels['B'].paste(pixels(dogs[state][0],2),(210,y+22))
panels['C']=panel('C','Static custom logo','Example artwork: personal D / DAVID mark',monogram(),[
    '32 x 128; 512 raw bytes per unique image.',
    'This is one possible custom design, not a fixed logo.',
    'A different image can be used on each half.'])
panels['D']=panel('D','Layer + modifier status','Sample content; actual local QMK font',status,[
    '32 x 128; five text columns, sixteen rows.',
    'Example state: BASE, modifier flags, Caps, RGB.',
    'Active-state highlighting is not implemented here.'])
panels['E']=panel('E','Lightweight slideshow','Example set: BASE / SYM / GAME',card(),[
    'Three 32 x 128 cards, 512 raw bytes each.',
    'Cycle slowly or select a card by layer.',
    'These cards are review concepts, not shipped art.'])
for j,(label,kind) in enumerate([('SYM','sym'),('GAME','game')]):
    panels['E'].paste(pixels(card(label,kind),2),(205+j*115,150))
panels['F']=panel('F','Stock Corne logo','Decoded from your local crkbd font + logo table',logo.transpose(Image.Transpose.ROTATE_90),[
    'Original is landscape: 126 x 24 visible pixels.',
    'Shown sideways in the physical portrait footprint.',
    'Raw bitmap version avoids linking the font table.'],logo)
for letter,im in panels.items(): im.save(ROOT/f'{letter}-preview.png')
sheet=Image.new('RGB',(1578,1480),'#080c11'); d=ImageDraw.Draw(sheet)
d.text((24,14),'WIRED CORNE / SIX OLED DIRECTIONS',fill=FG,font=ui_font(27,True))
d.text((24,53),'Actual source pixels where available. Custom cards are concepts. A needs adaptation.',fill=MUTED,font=ui_font(17))
for i,(letter,im) in enumerate(panels.items()): sheet.paste(im,(12+(i%3)*522,88+(i//3)*696))
sheet.save(ROOT/'six-options.png')
def gif(name,frames,durations):
    frames=[pixels(f,4) for f in frames]
    frames[0].save(ROOT/name,save_all=True,append_images=frames[1:],duration=durations,loop=0,disposal=2)
gif('A-bongo-source.gif',catframes,[200,200])
gif('B-luna.gif',[pet(s,i) for s in dogs for i in [0,1,0,1]],[250]*20)
gif('E-slideshow.gif',[card(),card('SYM','sym'),card('GAME','game')],[1800]*3)
for name,im in [('C-logo-32x128',monogram()),('D-status-32x128',status),('F-stock-128x32',logo)]: im.save(ROOT/(name+'.png'))
manifest={'bongo_array_bytes':{k:len(v) for k,v in bongo_data.items()},'luna_bytes_per_pose':{k:len(array(luna,k)) for k in dogs},'font_bytes':len(fontbytes),'sources':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in (ROOT/'sources').glob('*.c')}}
(ROOT/'source-manifest.json').write_text(json.dumps(manifest,indent=2))
print(json.dumps(manifest,indent=2))
