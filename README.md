# SSHPJ
Guida all'installazione:

requisiti:
disporre di una macchina fisica o virtuale con sistema operativo debian 10+ (eventualmente andrà bene una qualsiasi distribuzione, anche red hat o arch, ma i test sono stati eseguti su delle macchine debian).

passaggi:
GIT
1) installare git (https://www.html.it/pag/53180/installazione-di-git/)
2) creare una cartella che conterrà il repository:  "mkdir gitProj"
3) inizializzare la repository git dentro la cartella precedentemente creata:  "git init"
4) scaricare la repository: "git pull https://github.com/doclorenzo/SSHPJ.git"

DOCKER
installare docker seguendo la documemntazione ufficiale (https://docs.docker.com/engine/install/)
esempio per debian:

1) sudo apt update
2) sudo apt install apt-transport-https ca-certificates curl software-properties-common gnupg2
3) curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -
4) sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable"
5) sudo apt update
6) sudo apt install docker-ce
(sudo systemctl status docker)

IMMAGINI
installare le immagini delle due applicazioni:

1) docker pull influxdb:2.0
2) docker pull doclorenzo/unimoreweb

INSTALLAZIONE DEL SISTEMA
il passo finale è l'esecuzione dei due container, per farl bisogna dare i permessi di esecuzione ai file di setup e disinstallazione all'interno della cartella creata precedentemente (che contiene la repository).

1) sudo chmod 777 setup.sh
2) sudo chmod 777 uninstall.sh
3) (per configurare il sistema)   ./setup.sh
4) (per eliminare i container e la net)   ./uninstall.sh

