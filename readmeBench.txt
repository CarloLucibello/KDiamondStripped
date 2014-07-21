Un breve readme per l'uso da shell di comando

Guardando in main.cpp si possono osservare diverse modalità di lavoro (opzione -w) per il programma:

In tutti i casi i parametri fissi vengono presi dalla prima riga del file levels/params.txt
Tutti i range di variazione dei parametri sono in benchmarksuite.h

-w play 
è per fare una singola partita col giocatore intelligente
(senza funzione di benchmarksuite.cpp)

-w test
è per fare un test della maschera, a bias fissato, variando #colori, #mosse a disposizione, #punti obiettivo 
(funzione fullTestParam di benchmarksuite.cpp)

-w mask
è per fare un test delle diverse maschere tenendo fissi #colori, #mosse a disposizione, #punti obiettivo e bias
(funzione testParam di benchmarksuite.cpp con "mask" come primo argomento)


-w bias
è per fare un test della maschera tenendo fissi #colori, #mosse a disposizione, #punti obiettivo e variando il bias
(funzione testParam di benchmarksuite.cpp con "bias" come primo argomento)

-w "parametro generico x"
è per fare un test della maschera variando solo x
(funzione testParam di benchmarksuite.cpp con "x" come primo argomento)

--------------------------------------------------------------------
Esempio di riga di comando:
./kdiamond-stripped -w mask -v -i 100 -q 1
dove 
-w mask vedi sopra
-v è per output verbose
-i è per il numero (intero!) di iterazione del singolo livello 
-q è l'intelligenza del giocatore random (da 0. a 1.)

verrà generato:
simParam.txt con #iterazioni, valore di q e seed del generatore random 
maskParams.txt con i parametri del set di livelli simulati, in questo caso le diverse maschere (prima colonna)
maskres.txt con #livello #partita #punteggio #vittoria/sconfitta



