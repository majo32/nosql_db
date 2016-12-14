#!/bin/bash
function description(){
    echo 'NoSQL DBM systém implementovaný pomocou soketov (1)'
    echo 'Vytvorte jednoduchý NoSQL databázový systém v jazyku C, ktorý umožní aplikácii ukladať dvojicu údajov – kľúč a hodnotu. Obe položky sú reťazce znakov. Server bude prijímať príkazy pomocou soketov v inet doméne. Navrhnite vhodný komunikačný protokol pre výmenu údajov medzi serverom a klientom. Klientská časť umožní:'
    echo    
    echo '-- poslať na server novú dvojicu údajov, zadanú ako parameter'
    echo '-- opýtať sa na hodnotu asociovanú s daným kľúčom, zadaným ako parameter'
    echo '-- odstrániť dvojicu podľa zadaného kľúča, zadaného ako parameter'
    echo
    echo 'Údaje odoslané na server musia byť zachované aj v prípade ukončenia a opätovného spustenia servera'
    echo
    echo 'Marian Spisiak (spisiak3)'
    echo '5ZI038'
}

program='../build/server/FRI_OS_NOSQLDB_Server.exe'
program_kod='./server.c'
program_kod_db='./db.c'
dokumentacia='./doc.txt'

description
echo
echo "Chcete zobrazit dokumentacu? (y/n)"
read -n 1 buffer
echo
if [ "$buffer" = "y" ]
then
    less "$dokumentacia"
fi
echo "Vytváram program ..."
make
echo "Zadajte vstupne parametre(port):"
read buffer
"$program" "$buffer"
  
echo "Chcete zobrazit zdrojovy kod serveru ? (y/n)"
read -n 1 buffer
echo
if [ "$buffer" = "y" ]
then
    less "$program_kod"
fi

echo "Chcete zobrazit zdrojovy kod databazy ? (y/n)"
read -n 1 buffer
echo
if [ "$buffer" = "y" ]
then
    less "$program_kod_db"
fi
  
echo "Chcete zobrazit dokumentacu? (y/n)"
read -n 1 buffer
echo
if [ "$buffer" = "y" ]
then
   less "$dokumentacia"
fi