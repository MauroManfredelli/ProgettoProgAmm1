/*
 @file    main.cpp
 @author  Mauro Manfredelli 781266
 */

#include "cbuffer.h"
#include <iostream>

/**
 * predicato generico unario usato per testare il funzionamento
 * della funzione globale check in cbuffer.h
*/
struct predicato{
	
	/**
	 * verifica se un intero è pari
	 * @param x intero da controllare
	 * @return true se x è pari false altrimenti
	*/
	bool operator() (int x){
		return (x%2==0);
	}
	
	/**
	 * verifica se un double è minore di 5.5
	 * @param x double da controllare
	 * @return true se è minore di 5.5 false altrimenti
	*/
	bool operator() (double x){
		return (x>5.5);
	}
	
	/**
	 * verifica se un char è vocale
	 * @param x char da controllare
	 * @return true se è una vocale false altrimenti
	*/
	bool operator() (char x){
		if(x=='a'){
			return true;
		}
		else if(x=='e'){
			return true;
		}
		else if(x=='i'){
			return true;
		}
		else if(x=='o'){
			return true;
		}
		else if(x=='u'){
			return true;
		}
		else
			return false;
	}
	
	/**
	 * verifica se una stringa ha una lunghezza < 6
	 * @param x stringa da controllare
	 * @return true se è minore di 6 false altrimenti
	*/
	bool operator() (std::string x){
		return (x.length()<6);
	}
};

/**
 * Test sull'uso dei costruttori della classe cbuffer. In totale creo 5 cbuffer.
*/
void testCostructor(){
	cbuffer<double> cb(10, 1.2); 
	cbuffer<int> cb2(11,3);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	std::cout << "size: "<<cb2.get_size()<<" items: "<<cb2.get_items()<<" //" << cb2 << std::endl;
	cb=cb2;
	cbuffer<double> cb3(cb2);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	std::cout << "size: "<<cb3.get_size()<<" items: "<<cb3.get_items()<<" //" << cb3 << std::endl;
	
	cbuffer<char> cb1(5,'a');
	cbuffer<char> cb4(cb1);
	std::cout << "size: "<<cb4.get_size()<<" items: "<<cb4.get_items()<<" //" << cb4 << std::endl;
}

/**
 * Test sull'inserimento (insert) e cancellazione (del) dal buffer circolare.
*/
void testInsertDel(){
	cbuffer<float> cb(10,1.2);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.insert(2.4);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.insert(3.7);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.insert(4.8);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.del();
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.insert(5.1);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.del();
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.insert(6);
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.del();
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.del();
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
	cb.del();
	std::cout << "size: "<<cb.get_size()<<" items: "<<cb.get_items()<<" //" << cb << std::endl;
}

/**
 * Stampa per controllare l'utilizzo di funzioni const.
 * @param const cbuffer<int> buffer circolare da stampare.
*/
void stampa(const cbuffer<int> &cb){
	for(cbuffer<int>::size_type i=0; i<cb.get_items(); i++){
		std::cout << cb[i] << " " << cb.value(i) << " ";
	}
	std::cout << std::endl;
}

/**
 * Test per verificare la corretta esecuzione di vari metodi fondamentali. (utilizzo
 * di stampa come procedura d'appoggio).
*/
void testValueGetSetOperator(){
	cbuffer<int> cb1(5);
	cbuffer<float> cb2(4,2.2);
	cbuffer<std::string> cb3(3,"stringa");
	std::cout << "test get_size() e get_items(): " << std::endl;
	std::cout << "cb1 size: " << cb1.get_size() << std::endl;
	std::cout << "cb2 size: " << cb2.get_size() << std::endl;
	std::cout << "cb3 size: " << cb3.get_size() << std::endl;
	std::cout << "cb1 items: " << cb1.get_items() << std::endl;
	std::cout << "cb2 items: " << cb2.get_items() << std::endl;
	std::cout << "cb3 items: " << cb3.get_items() << std::endl;
	
	std::cout << "test get_cbuffer(size_type) e set_cbuffer(size_type, const &T):" << std::endl;
	cb2.set_cbuffer(3, 4);
	std::cout << "cb2[3] set 4 " << std::endl;
	std::cout << "cb2[3] get: " << cb2.get_cbuffer(3) << std::endl;
	
	std::cout << "test T& value(size_type index):" << std::endl;
	cb3.value(0)="fiorellino";
	std::cout << "scrittura cb3.value(0)"<< std::endl;
	std::cout << "lettura cb3.value(0)= " << cb3.value(0) << std::endl;
	
	std::cout << "test operator[]" << std::endl;
	cb3[2]="alberello";
	std::cout << "scrittura cb3[2] " << std::endl;
	std::cout << "lettra cb3[2] " << cb3[2] << std::endl;
	
	std::cout << "stampa(const cbuffer<int>)" << std::endl;
	cbuffer<int> cb4(6);
	cb4.insert(1);
	cb4.insert(2);
	cb4.insert(3);
	cb4.insert(4);
	std::cout << cb4 << std::endl;
	stampa(cb4);
}

/**
 * Test per verificare il lancio di eccezioni.
 * @param const cbuffer<float> buffer circolare
 * @throw VoidPosition se provo a accedere a una posizione vuota.
 * @throw IndexOutOfBound se index supera la dimensione del buffer circolare
*/
void helpEcceptions(const cbuffer<float> &cb){
	std::cout << "- eccezioni lettura con opertori:" << std::endl;
	try{
		std::cout << cb[3];
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		std::cout << cb.value(3);
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		std::cout << cb[6];
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		std::cout << cb.get_cbuffer(6);
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	std::cout << std::endl;
}

/**
 * Test per verificare il lancio di eccezioni.
 * @throw VoidPosition se provo a accedere a una posizione vuota.
 * @throw IndexOutOfBound se index supera la dimensione del buffer circolare
*/
void testEcceptions(){
	cbuffer<float> cb(5);
	cb.insert(0.55);
	cb.insert(0.44);
	helpEcceptions(cb);
	std::cout << "- eccezioni scrittura con opertori:" << std::endl;
	try{
		cb[3]=4;
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		cb.value(3)=4;
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		cb[6]=4;
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
	try{
		cb.set_cbuffer(6,3);
	}
	catch(std::runtime_error &e) {
		std::cout<<e.what()<<std::endl;
	}
}

/**
 * Test per verificare il funzionamento degli iterator
*/
void testIterator(){
	cbuffer<int> cb(10,1);
	cb.insert(3);
	cb.insert(1);
	cb.insert(2);
	cbuffer<int>::iterator bg=cb.begin();
	cbuffer<int>::iterator fn=cb.end();
	std::cout << cb << std::endl;
	int i=1;
	assert(bg!=fn);
	cb.del();
	bg=cb.begin();
	fn=cb.end();
	while(bg!=fn){
		*bg=i++;
		std::cout << *bg << " ";
		bg++;
	}
	std::cout << std::endl << "size: " << cb.get_size() << " items: " << cb.get_items() << std::endl << std::endl;
	
	cbuffer<std::string> cb1(5);
	cb1.insert("ciao");
	cb1.insert("come");
	cb1.insert("stai");
	cb1.insert("?");
	cb1.insert("bene");
	cbuffer<std::string>::iterator bg1=cb1.begin();
	cbuffer<std::string>::iterator fn1=cb1.end();
	assert(bg1!=fn1);
	while(bg1!=fn1){
		std::cout << *bg1 << " ";
		bg1++;
	}
	std::cout << std::endl << "size: " << cb1.get_size() << " items: " << cb1.get_items() << std::endl << std::endl;
	
	cbuffer<double> cb2(10);
	cbuffer<double>::iterator bg2=cb2.begin();
	cbuffer<double>::iterator fn2=cb2.end();
	assert(bg2==fn2);
	cb2.insert(3.8);
	cb2.insert(1.9);
	cb2.insert(2.5);
	bg2=cb2.begin();
	fn2=cb2.end();
	while(bg2!=fn2){
		std::cout << *bg2 << " ";
		bg2++;
	}
	std::cout << std::endl << "size: " << cb2.get_size() << " items: " << cb2.get_items() << std::endl;
}

/**
 * Procedura d'appoggio per il test di stampa con const_iteratorù
 * @param const cbuffer<float> buffer circolare da stampare.
*/
void stampaIterator(const cbuffer<float> &cb) {
	cbuffer<float>::const_iterator bg=cb.begin();
	cbuffer<float>::const_iterator fn=cb.end();
	while(bg!=fn){
		std::cout << *bg << " ";
		bg++;
	}
	std::cout << std::endl << "size: " << cb.get_size() << " items: " << cb.get_items() << std::endl;
}

/**
 * Procedura d'appoggio per il test del confronto tra itearator e const_iterator
*/
void confrontoIterator(cbuffer<float> &cb, const cbuffer<float> &cb1){
	cbuffer<float>::iterator bg=cb.begin();
	cbuffer<float>::const_iterator bg1=cb1.begin();
	cbuffer<float>::iterator fn=cb.end();
	cbuffer<float>::const_iterator fn1=cb1.end();
	while((bg!=fn) and (bg1!=fn1)){
		assert(bg!=bg1);
		//assert(bg==bg1);
		bg++;
		bg1++;
	}
	stampaIterator(cb1);
}

/**
 * Test per verificare il funzionamento dei const_iterator con procedure d'appoggio
*/
void testConstIterator() {
	cbuffer<float> cb(5);
	cb.insert(2.1);
	cb.insert(1.5);
	cb.insert(3.0);
	stampaIterator(cb);
	cbuffer<float> cb1(4,3);
	confrontoIterator(cb,cb1); //da testare con assert(bg!=bg1) in confrontoIterator
	//confrontoIterator(cb,cb);  //da testare con assert(bg==bg1) in confrontoIterator
}

/**
 * Test per verificare il corretto funzionamento della funzione globale check
*/
void testCheck(){
	predicato pred;
	cbuffer<int> cb(10,2);
	cb[0]=1;
	check(cb, pred);
	cbuffer<double> cb1(10,2.5);
	cb1[2]=6;
	check(cb1, pred);
	cbuffer<std::string> cb2(10,"bomber");
	check(cb2, pred);
	cbuffer<char> cb3(10,'x');
	cb3.insert('a');
	cb3[0]='e';
	cb3[6]='u';
	check(cb3,pred);
}

int main() {
	std::cout << "---test costruttori:---" << std::endl;
	testCostructor();
	std::cout << std::endl;
	
	std::cout << "---test di insert e del:---" << std::endl;
	testInsertDel();
	std::cout << std::endl;
	
	std::cout << "---test su vari metodi fondamentali:--- "<< std::endl;
	testValueGetSetOperator();
	std::cout << std::endl;
	
	std::cout << "---test eccezioni:---" << std::endl;
	testEcceptions();
	std::cout << std::endl;
	
	std::cout << "---test degli iteratori:---" << std::endl;
	testIterator();
	std::cout << std::endl;
	
	std::cout << "---test dei const_iterator:---" << std::endl;
	testConstIterator();
	std::cout << std::endl;
	
	std::cout << "---test della funzione gloabale check:---" << std::endl;
	testCheck();
	std::cout << std::endl;
	
	return 0;
}
