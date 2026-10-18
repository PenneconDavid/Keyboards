# known-good firmware

Recovery binaries only. Prefer these over rebuilding when a board must go back to a last-working state. These are the only copies in the repo (the duplicates under `archive/` and `docs/` were removed 2026-10-17 after a sha256 check).

| Folder | Keyboard | Notes |
|---|---|---|
| `wired-corne/` | QMK Crkbd rev1 | VIA hex of the old 4-layer keebv2 (pre-v4.1 port); the matching live layout is `layouts/via/crkbd.layout.2026-05-10.json` |
| `sofle-dongle/` | Eyelash Sofle ZMK | Vendor dongle + nice!view halves |
| `cheapino/` | Vial UF2 | No source in this repo |

Wireless Eyelash Corne rollback UF2s are in `ZmkConfig/known-good/eyelash-corne/`.
