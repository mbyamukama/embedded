avrdude -p m256rfr2 -c stk500v2 -P "COM10" -b 38400 -e  -U flash:w:sendstring.hex