"""Verify compiled HPD defaults, UF2 family/addresses and ELF-to-UF2 contents."""
import hashlib
import json
import shutil
import struct
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'.build/hpd-v2/vial-qmk'
OUT=ROOT/'firmware-out/hpd-v2/pc-tv-v3'
AUDIT=ROOT/'docs/builds/hpd-v2/2026-10-25'
STEM='ergohaven_hpd_rev2_pc_tv_v3'
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
expected=json.loads((AUDIT/'vial-decoded-keycodes.json').read_text(encoding='utf-8'))
assert sha(OUT/'hpd_v2_pc_tv_v3.vil') == expected['vial_sha256'], 'Vial import changed after independent parsing'
elf=BUILD/'.build'/f'{STEM}.elf'
uf2=BUILD/'.build'/f'{STEM}.uf2'
if not uf2.exists(): uf2=BUILD/f'{STEM}.uf2'
blob=elf.read_bytes()
assert blob[:7] == b'\x7fELF\x01\x01\x01', 'Expected little-endian ELF32'
eh=struct.unpack_from('<16sHHIIIIIHHHHHH',blob)
assert eh[2] == 40, 'Expected ARM ELF'
sections=[struct.unpack_from('<IIIIIIIIII',blob,eh[6]+i*eh[11]) for i in range(eh[12])]
symbols={}
for sec in sections:
    if sec[1] != 2: continue
    strings=sections[sec[6]]
    names=blob[strings[4]:strings[4]+strings[5]]
    for off in range(sec[4],sec[4]+sec[5],sec[9]):
        sym=struct.unpack_from('<IIIBBH',blob,off)
        name=names[sym[0]:].split(b'\0',1)[0].decode()
        symbols[name]=sym
def symbol_data(name):
    sym=symbols[name]
    sec=sections[sym[5]]
    off=sec[4]+sym[1]-sec[3]
    return blob[off:off+sym[2]]
# Vial serializes false/true (CCW/CW); ENCODER_CCW_CW stores CW first in C.
# Compare in the compiled table's storage order, retaining the physical directions.
assert '{ (cw), (ccw) }' in (BUILD/'quantum/encoder.h').read_text(encoding='utf-8')
encoder_storage=[value for i in range(0,len(expected['encoder']),2)
                 for value in [expected['encoder'][i+1],expected['encoder'][i]]]
for name,values in [('keymaps',expected['keymap']),('encoder_map',encoder_storage)]:
    target=struct.pack('<'+'H'*len(values),*values)
    actual=symbol_data(name)
    assert actual == target, f'{name}: {len(actual)} compiled bytes != {len(target)} expected bytes; actual={actual.hex()} expected={target.hex()}'
assert symbols['keyboard_post_init_user'][2] > 0

raw=uf2.read_bytes()
assert len(raw)%512 == 0
blocks=len(raw)//512
payload={}
addresses=[]
for index in range(blocks):
    block=raw[index*512:(index+1)*512]
    m0,m1,flags,address,size,number,total,family=struct.unpack_from('<8I',block)
    assert (m0,m1,struct.unpack_from('<I',block,508)[0]) == (0x0A324655,0x9E5D5157,0x0AB16F30)
    assert flags == 0x2000 and family == 0xE48BFF56, 'Not an RP2040 UF2'
    assert number == index and total == blocks and size == 256 and address%256 == 0
    assert 0x10000000 <= address and address+size <= 0x101FC000, 'Overlaps reserved EEPROM region or invalid flash'
    assert address not in addresses, 'Overlapping UF2 blocks'
    addresses.append(address)
    payload.update({address+j:byte for j,byte in enumerate(block[32:32+size])})

compared=0
for i in range(eh[10]):
    ph=struct.unpack_from('<8I',blob,eh[5]+i*eh[9])
    if ph[0] != 1 or not ph[4] or not 0x10000000 <= ph[3] < 0x10200000: continue
    data=blob[ph[1]:ph[1]+ph[4]]
    assert bytes(payload[ph[3]+j] for j in range(ph[4])) == data, 'ELF/UF2 load-segment mismatch'
    compared+=len(data)
assert compared > 0
log=ROOT/'.build/hpd-v2/build-pc-tv-v3.log'
logtext=log.read_text(encoding='utf-8-sig')
assert 'error:' not in logtext.lower() and 'error 1' not in logtext.lower()
assert 'warning:' not in logtext.lower(), 'Review compiler warnings before packaging'

AUDIT.mkdir(parents=True,exist_ok=True)
shutil.copyfile(elf,AUDIT/f'{STEM}.elf')
shutil.copyfile(log,AUDIT/'build.log')
published=OUT/'hpd_v2_pc_tv_v3.uf2'
shutil.copyfile(uf2,published)
# QMK also copies the generated file to the userspace root. Retain one delivery
# copy; only remove that exact generated path when it is byte-identical.
extra=ROOT/f'{STEM}.uf2'
if extra.exists() and sha(extra) == sha(published): extra.unlink()
report={
    'date':'2026-10-25','target':'ergohaven/hpd/rev2:pc_tv_v3',
    'upstream_revision':'98bf33054c73619b590505853d8d8f3e0557f41e',
    'keymap_entries_matched':len(expected['keymap']),'encoder_entries_matched':len(expected['encoder']),
    'encoder_order':'Vial CCW/CW normalized to compiled CW/CCW',
    'startup_policy_hook_present':True,'uf2_family':'RP2040','uf2_family_id':'0xe48bff56',
    'uf2_blocks':blocks,'uf2_file_bytes':len(raw),
    'flash_start':hex(min(addresses)),'flash_end_exclusive':hex(max(addresses)+256),
    'reserved_storage_start':'0x101fc000','elf_flash_bytes_compared':compared,
    'uf2_sha256':sha(published),'vial_sha256':expected['vial_sha256'],
    'elf_sha256':sha(elf),'compiler_warnings':0,'hardware_tested':False,'flashed':False,
}
(AUDIT/'verification.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
manifest=json.loads((OUT/'source-manifest.json').read_text(encoding='utf-8'))
manifest['status']='compiled and statically verified; hardware identification/backup/test pending'
manifest['verification']=report
(OUT/'source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
(OUT/'SHA256SUMS').write_text('\n'.join(f'{sha(p)}  {p.name}' for p in [published,OUT/'hpd_v2_pc_tv_v3.vil'])+'\n',encoding='ascii')
print(json.dumps(report,indent=2))
