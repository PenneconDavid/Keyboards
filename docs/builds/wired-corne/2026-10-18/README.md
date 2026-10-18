# Wired Corne build records

The latest approved flashable HEX lives in `firmware-out/` at the repository root.
`approved-firmware.json` records the user's 2026-10-18 approval and its SHA-256.
Older HEX candidates are retained under `archive/firmware/wired-corne/2026-10-18/`.

ELF/debug files, original checksum companions, logs and build manifests remain here. The original
manifests describe build-time state, including their then-unverified hardware status, and name
artifacts by basename. Approval is recorded separately rather than rewriting that evidence.
Checksum companions also retain their original basenames; locate each corresponding HEX in
`firmware-out/` (approved orientation fix) or the archive (superseded candidates).

`cleanup-move-inventory.json` records the original and current paths and before/after hashes for
the relocated files. Supporting documentation was updated after the initial hash-verified move.
No firmware binary or retained build manifest was modified.
