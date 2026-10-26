# Keymap kronann (Corne)

QWERTY, 4 couches, mêmes codes sur macOS et Linux. Le firmware envoie des codes japonais (`JP_*`) pour tous les symboles, et les deux OS lisent le clavier en JIS : chaque symbole du schéma sort tel quel. Le japonais passe par l'IME (Ctrl+Espace sous Linux ; sur macOS, la bascule reste à régler).

Schéma des couches : [`layout.html`](layout.html). Configuration macOS qui va avec : [`macos/`](macos/).

Chaque couche se lit : 6 touches gauches, `[intérieure gauche]`, `[intérieure droite]`, 6 touches droites. `▽` = transparente, `·` = aucune action.

## Couches

### BASE

```
Tab   Q W E R T  [←] [↑]  Y U I O P Bksp
⇧     A S D F G  [→] [↓]  H J K L ; '
Ctrl  Z X C V B           N M , . / Esc
        Hyper LOWER Espace   Entrée RAISE Alt
```

Tab = Tab si on tape, Ctrl si on maintient. Hyper = Super gauche. Shift + Bksp = Suppr. Shift façon US : Shift + `'` = `"`, Shift + `;` = `:`, Shift + `/` = `\`, Shift + `,` = `<`, Shift + `.` = `>`.

### LOWER (pouce gauche central)

```
!     1 2 3 4 5  ▽ [WS←]  6 7 8 9 0 Suppr
⇧     # { [ ( =  ▽ [WS→]  - ) ] } $ _
Ctrl  ` @ & < |           ^ > * % + ~
        ▽ ▽ ABC⇄あ         ▽ ▽ ▽
```

WS←/WS→ = Super+PgUp / Super+PgDn. Hyper + LOWER + chiffre 1–5 = Super+chiffre : affiche le workspace N (skhd sur Mac, Hyprland sous Linux). ABC⇄あ = Ctrl+Espace. Shift + `!` = `?`. Les ouvrants et fermants sont en miroir sur la rangée de repos.

### RAISE (pouce droit central)

```
▽     · · ↑ · ·       ▽ ▽  Début PgDn PgUp Fin Impr ▽
⇧     Mol↑ ← ↓ → Lent ▽ ▽  ← ↓ ↑ → · ·
Ctrl  Mol↓ ClicD ClicM ClicG ·   Muet Vol− Vol+ Lum− Lum+ ▽
        ClicD LOWER ClicG      ▽ ▽ ▽
```

Les flèches et les clics de la main gauche pilotent la souris.

### ADJUST (LOWER + RAISE ensemble)

```
F1    F2 F3 F4 F5 F6  ·  ·  F7 F8 F9 F10 F11 F12
RGB   Teinte+ Sat+ Lum+ Effet+ ·   ·  ·  · · · · · ·
Boot  Teinte− Sat− Lum− Effet− ·   ·  ·  · · · · · Boot
```

Boot est sur les deux coins du bas, pour flasher chaque moitié.

## Compiler et flasher

```bash
qmk compile -kb crkbd/rev4_1/standard -km kronann
```

Flasher chaque moitié : ADJUST + Boot, puis copier `crkbd_rev4_1_standard_kronann.uf2` sur le volume qui apparaît.

## Configuration OS (en place, copiée dans `macos/`)

Les deux OS lisent le clavier en JIS.

- **macOS** : `keyboard_type_v2 = "jis"` dans `~/.config/karabiner/karabiner.json`.
- **Linux** : `kb_layout = "jp,fr,fr(ergol),us"` dans Hyprland, donc jp par défaut.
- **Karabiner** : règle limitée au Corne (`vendor_id` / `product_id` du crkbd) qui transforme Ctrl + C V X Z A S F T W N Q R L P en ⌘ + la même touche, sauf si l'application au premier plan est kitty (`net.kovidgoyal.kitty`) ou UTM/Parallels. Les remaps 英数 → ⌥ gauche, かな → 英数 et Verr Maj → ⌘ droit visent tous les claviers (`is_keyboard: true`) ; le Corne n'envoie aucune de ces touches. La règle globale « ⌘ gauche → Hyper » reste globale.
- **Karabiner, `\`** : macOS JIS donne `_` pour la touche ろ ; une règle limitée au Corne transforme ろ sans modificateur en ⌥ droit + ¥, qui donne `\`.
- **kitty** : `macos_option_as_alt left`, pour que ⌥ droit reste une Option macOS et que ⌥¥ donne `\` (⌥ gauche reste Alt).
- **skhd** : `ctrl + alt + cmd - 1` à `5` et `pageup`/`pagedown` pilotent les Spaces via yabai (voir `macos/skhdrc`).
- **Raycast** : la touche Hyper de Raycast reste sur None, sinon le ⌘ émis par la règle Ctrl du Corne devient Hyper.
- **Copies versionnées** : `macos/` contient `karabiner.json`, `skhdrc`, `scratchpad_toggle.sh` et `cycle_clockwise.sh`, copiés depuis `~/.config`. Après chaque modification de la config, recopier les fichiers et contrôler avec `diff`.
