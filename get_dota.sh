mkdir -p dota

FILEID=1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK
FILENAME=part1.zip
wget --load-cookies cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=$FILEID' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=$FILEID" -O $FILENAME && rm -rf cookies.txt

unzip part1.zip -d dota && rm part1.zip

FILEID=1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv
FILENAME=part2.zip
wget --load-cookies cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=$FILEID' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=$FILEID" -O $FILENAME && rm -rf cookies.txt

unzip part2.zip -d dota && rm part2.zip