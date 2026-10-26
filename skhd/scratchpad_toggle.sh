#!/bin/bash

SCRATCHPAD_SPACE=5

# Get current space (with fallback)
CURRENT_SPACE=$(yabai -m query --spaces --space 2>/dev/null | jq -r '.index // empty')

# Si pas de space actif, on sort
if [ -z "$CURRENT_SPACE" ]; then
    exit 0
fi

if [ "$CURRENT_SPACE" -eq "$SCRATCHPAD_SPACE" ]; then
    # On est dans le scratchpad → retourne au space précédent
    yabai -m space --focus recent
else
    # Check si des fenêtres existent dans le scratchpad
    WINDOWS_IN_SCRATCHPAD=$(yabai -m query --windows --space "$SCRATCHPAD_SPACE" 2>/dev/null | jq 'length // 0')

    # Get window ID (peut être null)
    WINDOW_ID=$(yabai -m query --windows --window 2>/dev/null | jq -r '.id // empty')

    # Si scratchpad vide ET on a une fenêtre active, l'envoyer au scratchpad
    if [ "$WINDOWS_IN_SCRATCHPAD" -eq 0 ] && [ -n "$WINDOW_ID" ]; then
        yabai -m window --space "$SCRATCHPAD_SPACE"
    fi

    # Toggle vers le scratchpad
    yabai -m space --focus "$SCRATCHPAD_SPACE"
fi
