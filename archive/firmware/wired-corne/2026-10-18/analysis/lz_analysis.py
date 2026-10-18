exec(open('docs/oled-previews/verification/compression_analysis.py').read().split('for name,data')[0])
def lz(data):
 out=[];i=0
 while i<len(data):
  group=[];flag=0
  for bit in range(8):
   if i>=len(data):break
   best=(0,0)
   for off in range(1,min(i,255)+1):
    n=0
    while n<min(258,len(data)-i) and data[i+n]==data[i+n-off]:n+=1
    if n>=3 and n>best[0]:best=(n,off)
   n,off=best
   if n>=3:flag|=1<<bit;group.extend([off,n-3]);i+=n
   else:group.append(data[i]);i+=1
  out.extend([flag]+group)
 return bytes(out)
print('LZSS 255win 3..258ref cards',sum(len(lz(c)) for c in cards),'luna',sum(len(lz(c)) for c in sprites))
print('with shared inter-card window', len(lz(b''.join(cards))))
