#!/bin/bash
bash build.sh || exit 1
bash inject_debug_via_ldpreload.sh
