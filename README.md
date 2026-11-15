# QMK Userspace

My QMK keymaps, built as an external userspace. For setup, GitHub builds and the `qmk_firmware` submodule option, refer to the [QMK userspace template](https://github.com/qmk/qmk_userspace#readme).

- Keymaps: `keyboards/<keyboard>/keymaps/rpbaptist/`. Each has a README.
- Build locally: `qmk config user.overlay_dir="$(realpath .)"`, then `qmk compile -kb <keyboard> -km rpbaptist`.
