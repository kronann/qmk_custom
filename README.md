# qmk_custom

Userspace QMK du clavier Corne (crkbd rev4.1) : keymap `kronann`, QWERTY en 4 couches, mêmes codes sur macOS et Linux. Détails et schéma : [`keyboards/crkbd/keymaps/kronann/`](keyboards/crkbd/keymaps/kronann/readme.md).

Configuration macOS qui va avec, copiée depuis `~/.config` : [`karabiner/`](karabiner/) et [`skhd/`](skhd/).

## Compiler

Il faut un `qmk_firmware` installé (`qmk setup`). Une fois, indique où il se trouve :

```bash
qmk config user.qmk_home=<chemin de qmk_firmware>
```

Puis, depuis ce dossier :

```bash
qmk compile -kb crkbd/rev4_1/standard -km kronann
```

QMK reconnaît le userspace grâce à `qmk.json` et dépose `crkbd_rev4_1_standard_kronann.uf2` ici. Flasher chaque moitié : ADJUST + Boot, puis copier le `.uf2` sur le volume qui apparaît.
