#!/bin/bash
make clean; git add -A; git commit -m "m"; git push; make; pm2 restart all
