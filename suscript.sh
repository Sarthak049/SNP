gcc -o sample sample.c `pkg-config --cflags --libs gtk+-2.0`
gcc -o application application.c
gcc -o temp temp.c
sudo chown root temp
sudo chmod u+s temp
sudo chown root application
sudo chmod u+s application
sudo mkdir /home/cryptomod
./sample
