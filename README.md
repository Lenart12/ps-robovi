# ps-robovi

# Odkrivanje robov v slikah
Odkrivanje robov predstavlja enega temeljnih postopkov pri procesiranju slik [7]. Z nje-
govo pomočjo iz slike izluščimo informacijo o prisotnih strukturah. Uporablja se v po-
stopkih računalniškega vida, kot na primer razpoznavanje objektov, detekcijo voznega
pasu, razpoznavanje prstnih odtisov, obdelava medicinskih slik. Razvitih je bilo veliko
metod za odkrivanje robov, ena od bolj znanih je Cannyev detektor [8].

## 2.1 Cannyev detektor
Postopek odkrivanja robov je sestavljen iz petih korakov:

1. Glajenje oziroma odstranjevanje šuma z Gaussovim filtrom
Odkrivanje robov je zelo občutljivo na šum v sliki, zato je potrebno sliko predhodno
zgladiti. Sliko najprej pretvorimo v sivinsko, nato pa izvedemo glajenje. Običajno
se uporablja Gaussov filter
G(x, y) = 1
2πσ2 e− x2+y2
2σ2
z velikostjo okna 5x5, ki deluje dobro v večini primerov. Parameter σ izberemo na
intervalu [1, 2].
2. Računanje gradienta slike.
V tem koraku izračunamo odvod intenzitete v sliki v navpični in vodoravni smeri.
To naredimo s pomočjo Sobelovih operatorjev, tako da z njimi naredimo konvolucijo
nad sliko.
Gx =



1 0 −1
2 0 −2
1 0 −1


 ∗ I Gy =



1 2 1
0 0 0
−1 −2 −1


 ∗ I,
kjer je ∗ operator konvolucije v dveh dimenzijah, I vhodna slika, Gx in Gy pa
sliki, ki vsebujeta približek gradienta za vsako točko slike I v vodoravni (Gx) in
navpični (Gy) smeri. Na podlagi Gx in Gy lahko v vsaki točki izračunamo magnitudo
G = √
G2
x + G2
y in smer gradienta intenzitete Θ = arctan2(Gx, Gy).
3. Tanjšanje robov
Po drugem koraku se na sliki magnitud gradienta vidijo posamezni robovi, vendar so
precej razmazani. V tem koraku poskusimo robove stanjšati in ohraniti le lokalne
maksimume. To naredimo s pomočjo metode izločanja neizrazitih slikovnih točk
(angl. Non-Maximum suppression).
4. Dvojno upragovanje
Po tretjem koraku smo dobili občutno tanjše robove, vendar je v sliki še vedno
precej razlik v intenziteti posameznih robov. To bomo odpravili z zadnjima dvema
4
korakoma. S pomočjo dvojnega upragovanja razvrstimo slikovne točke v tri kate-
gorije: izrazite, šibke in nepomembne. Izberemo si dve mejni vrednosti, visoko in
nizko. Slikovne točke z magnitudo gradienta višjo od visoke mejne vrednosti ozna-
čimo kot izrazite, slikovne točke med obema mejnima vrednostima kot šibke, ostale
pa kot nepomembne. Mejni vrednosti določimo empirično.
5. Sledenje robovom s pomočjo histereze
Izrazite točke, ki smo jih dobili v prejšnjem koraku, bodo del robov v izhodni sliki,
medtem ko bomo nekatere izmed šibkih slikovnih točk v tem koraku spremenili v
izrazite po naslednjem merilu. Za vsako šibko slikovno točko pogledamo njeno ne-
posredno okolico (8 sosednjih slikovnih točk), če je katerakoli od sosednjih slikovnih
točk izrazita, spremenimo trenutno šibko točko v izrazito sicer pa točko označimo za
nepomembno. Izhodno sliko sestavljajo izrazite točke, ki jim nastavimo intenziteto
na najvišjo možno vrednost. Ostalim slikovnim točkam pa nastavimo intenziteto
na 0.
(a) Vhodna slika. (b) Izhodna slika.
Slika 1: Odkrivanje robov v sliki s Cannyevim detektorjem.
Podroben opis posameznik korakov in izvorno kodo v jeziku Python najdete na pove-
zavi. Za branje in zapisovanje slik lahko uporabite ustrezno knjižnico, na primer STB [9]
ali FreeImage [10]. V kolikor želite, lahko uporabite knjižnico OpenCV, ki že vsebuje im-
plementacije nekaterih funkcij (e.g. Sobelov operator, branje, pisanje slik, itd.). Primer
uporabe OpenCV v jeziku Python za odkrivanje robov najdete na povezavi. Knjižnico
OpenCV lahko uporabite v kombinaciji z jezikom C/C++, navodila za namestitev naj-
dete tukaj.

## 2.2 Naloga
Na voljo imamo večje število slik, v katerih želimo poiskati robove. Postopek želimo
čim bolj pohitriti. Iskanje robov v sliki je sestavljeno iz petih zgoraj navedenih korakov
ter branja vhodne ter zapisovanja izhode slike. Izvajanje lahko organiziramo kot cevo-
vod, kjer posamezne stopnje cevovoda predstavljajo posamezne korake odkrivanja robov.
Počasnejše stopnje lahko vzporedno izvajamo na več procesorskih jedrih.
Pripravite dve rešitvi ali eno hibridno, ki bodo uporabljale knjižnici Pthread in MPI.
Komunikacijo med stopnjami pripravite po konceptu proizvajalec-porabnik. V osnovni
različici ni potrebno, da je vrstni red slik na izhodu enak kot na vhodu. Nekaj idej:
* Izmerite čase procesiranja na posameznih stopnjah cevovoda, izračunajte pohitritve,
učinkovitost.
* Koncept proizvajalec-porabnik razširite tako, da bo proizvajalec naloge lahko od-
dajal v medpomnilnik.
* Preverite, koliko pomaga uporaba več vzporednih procesorjev v izbranih stopnjah
cevovoda. Lahko obdelujete več slik naenkrat v isti stopnji cevovoda ali pa upora-
bite več jeder za obdelavo ene slike.
* Za koliko se zmanjšajo pohitritve, če zahtevate, da je vrstni red slik na izhodu enak
tistemu na vhodu (obdelava videa)?
* Program izboljšajte tako, da število vzporednih procesorjev na vsaki stopnji dolo-
čate dinamično, glede na potrebe.
