# Programmazione di sistema
#### Anno accademico 2018-2019
### Esercitazione 5
Si realizzi, utilizzando la **libreria Qt**, un analizzatore grafico di uso del disco che sia in grado di scansionare l'intero albero del file system o uno specifico ramo di directory richiesto dall'utente. L'interfaccia è composta dai seguenti elementi:
+ Una barra che mostra la porzione di disco occupata e la capacità totale del disco, in GB;
+ una casella editabile con capacità di autocompletamento che permette di selezionare il path da analizzare
	+ vengono suggerite solo le entry di tipo directory il cui nome corrisponde parzialmente all'input dell'utente;
	+ l'input viene accettato con un click su una delle voci suggerite oppure con la pressione del tasto return;
+ un bottone per risalire l'albero delle directory fino eventualmente alla directory radice;
+ un grafico a torta che mostra lo spazio occupato da ciascun elemento presente nel path selezionato rispetto alla dimensione totale corrispondente al path stesso;
	+ al passaggio del mouse su uno spicchio del grafico, viene mostrato il nome dell'elemento corrispondente;
	+ la legenda del grafico evidenzia con colore diverso le directory presenti nel path esaminato
	+ Il click su uno spicchio o su un elemento della legenda di tipo directory, aggiorna la visualizzazione del grafico a torta con i dati relativi alla nuova directory selezionata.

E' possibile sfruttare, tra le molte classi offerte dalla libreria Qt, le seguenti:
+ **QChartView**, che permette di visualizzare grafici di vario tipo, compresi quelli a barre (QHorizontalStackedBarSeries) e a torta (QPieSeries)
+ **QMainWindow**, che fornisce un modo conveniente per organizzare l'intera interfaccia
+ **QLineEdit**, che offre una semplice interfaccia per leggere una riga di testo
+ **QCompleter**, che permette di gestire l'autocompletamento nelle caselle di testo appoggiandosi ad un modello di dati esterno (può essere utile estendere la classe QFileSystemModel per filtrare le entry di tipo di directory)
+ **QStorageInfo**, per accedere alle informazioni relative al file system

Ciascuna di queste classi mette a disposizione vari segnali volti a semplificare l'interazione dell'utente. Possono risultare interessanti i seguenti:
+ **QPieSeries::hovered** per mostrare l'etichetta al passaggio del mouse sul grafico
+ **QPieSeries::clicked** per aggiornare lo stato dell'applicazione, se è stata scelta una directory
+ **QLegendMarker::hovered** e **QLegendMarker::clicked** per rendere interattiva anche la legenda del grafico.

Si può fare uso della macro **QT_CHARTS_USE_NAMESPACE** in cima al file contenente la classe principale per rendere più concisa la sintassi.

#### Compilare il progetto con cmake
Modificare il file CmakeLists.txt del proprio progetto basandosi sull'esempio che segue.
```c++
cmake_minimum_required( VERSION 3.14 )
project( es5 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

find_package( Qt5Core REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Charts REQUIRED )
list( APPEND LIBRARIES
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Charts )
list( REMOVE_DUPLICATES LIBRARIES )

add_executable( es5 main.cpp MyMainWindow.h MyFileSystemModel.h MyWhateverOtherFile.h )
target_link_libraries( es5 ${ LIBRARIES } )
```

#### Competenze da acquisire
+ Uso della libreria Qt per la progettazione e realizzazione di interfacce grafiche
+ Comprensione dei meccanismi di signal e slot per la comunicazione tra oggetti