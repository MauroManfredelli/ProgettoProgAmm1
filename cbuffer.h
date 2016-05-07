/*
 @file    cbuffer.h
 @author  Mauro Manfredelli 781266
 @version 1.0
 */

#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream> // std::ostream
#include <iostream> // std::cout
#include <cassert> // assert
#include <stdexcept> // std::runtime_error

/**
 * Eccezione custom per gestire l'accesso a posizioni vuote
*/
class VoidPosition : public std::runtime_error{
public:
	VoidPosition(const char *msg) : std::runtime_error(msg) {}
};

/**
 * Eccezione custom per gestire l'accesso a posizioni fuori range
*/
class IndexOutOfBound : public std::runtime_error{
public:
	IndexOutOfBound(const char *msg) : std::runtime_error(msg) {}
};

/**
 * Classe che rappresenta un array/buffer di tipi generici circolare di dimensione fissa.
 * NOTA: la classe contiene delle stampe di debug che possono essere disabilitate 
 *	     aggiungendo il flag -DNDEBUG in fase di compilazione.
*/

template <typename T>
class cbuffer {
public:
	/** Nome del tipo di dato associato alla dimensione dell'array e al numero di items */
	typedef unsigned int size_type;
	
private:
	size_type size; ///<dimensione del buffer circolare
	size_type items; ///<indice che tiene il conto di quanti item ci sono nel buffer
	T* cb; ///<puntatore alla coda FIFO (buffer circolare)
	
	/**
	 * metodo privato d'appoggio per mantenere l'elemento più vecchio
	 * in testa.
	 */	
	void shift_left(){
		cbuffer tmp(this->size);
		for(size_type i=1; i<this->items; i++){
			tmp.insert(cb[i]);
		}
		std::swap(cb,tmp.cb);
		std::swap(size,tmp.size);
		std::swap(items,tmp.items);
	}
	
public:
	/**
	 *	1° METODO FONDAMENTALE: Costruttoe di default
	*/
	cbuffer(void) : cb(0), size(0), items(0) {
		#ifndef NDEBUG
			std::cout << "cbuffer::cbuffer()" <<std::endl;
		#endif
	}
	
	/**
	 * Costruttore secondario che prende la dimensione
	 * del buffer come parametro e lo inizializza a quella
	 * dimensione.
	 * explicit previene l'uso del costruttore come conversione di tipo (intero->cbuffer).
	 * @param sz dimensione del buffer
	*/
	explicit cbuffer(size_type sz) : cb(0), size(0), items(0) {
		#ifndef NDEBUG
			std::cout << "cbuffer::cbuffer(size_type sz)" <<std::endl;
		#endif
		
		cb=new T[sz];
		size=sz;
	}
	
	/**
	 * Costruttore secondario che inizializza il buffer a una
	 * certa dimensione e lo inizializza con certi valori init.
	 * @param sz dimensione del buffer
	 * @param init valore di default
	*/
	cbuffer(size_type sz, const T &init) : cb(0), size(0), items(0) {
		#ifndef NDEBUG
			std::cout << "cbuffer::cbuffer(size_type sz, const T &init)" <<std::endl;
		#endif
		
		cb=new T[sz];
		try{
			for(size_type i=0;i<sz;++i){
				cb[i]=init;
			}
		}catch(...){
			delete[] cb;
			cb=0;
			throw;
		}
		size=sz;
		items=sz;
	}
	
	/**
	 * 2° METODO FONDAMENTALE : distruttore
	*/
	~cbuffer(void){
		#ifndef NDEBUG
			std::cout<<"cbuffer::~cbuffer()"<<std::endl;
		#endif
		
		delete[] cb;
		cb=0;
		size=0;
		items=0;
	}
	
	/**
	 * 3° METODO FONDAMENTALE: copy constructor
	 * @param other cbuffer da copiare.
	*/
	cbuffer(const cbuffer &other) : cb(0), size(0), items(0){
		#ifndef NDEBUG
			std::cout<<"cBufer::cbuffer(const cbuffer &other)"<<std::endl;
		#endif
		
		cb=new T[other.size];
		try{
			for(size_type i=0; i<other.size;++i)
				cb[i]=other.cb[i];
		}catch(...){
			delete[] cb;
			cb=0;
			throw;
		}
		size=other.size;
		items=other.items;
	}
	/**
	 * copy cnstuctor con cbuffer contenente elementi di tipo diverso
	 * rispetto a this.
	 * @param other cbuffer<Q> da copiare
	*/
	template <typename Q>
	cbuffer(const cbuffer<Q> &other) : cb(0), size(0), items(0) {
		#ifndef NDEBUG
			std::cout<<"cBufer::cbuffer(const cbuffer<Q> &other)"<<std::endl;
		#endif
		
		cb=new T[other.get_size()];
		try {
			for (size_type i=0;i<other.get_size();++i)
				cb[i] = static_cast<T>(other[i]);
		}
		catch(...) {
			delete[] cb;
			cb=0;
			throw;
		}
		this->size=other.get_size();
		this->items=other.get_items();
	}
	
	/**
	 * 4° METODO FONDAMENTALE: operatore assegnamento
	 * @param other cbuffer da copiare
	*/
	cbuffer& operator=(const cbuffer &other){
		#ifndef NDEBUG
			std::cout<<"cbuffer::operator=(const cbuffer &other)"<<std::endl;
		#endif
		
		if(this != &other) {	
			cbuffer tmp(other);

			std::swap(cb,tmp.cb);
			std::swap(size,tmp.size);
			std::swap(items,tmp.items);
		}
		
		return *this;
	}
	
	/**
	 * operatore assegnamento per cbuffer con dati di tipo diverso
	 * @pram other cbuffer<Q> cbuffer da copiare
	*/ 
	template<typename Q>
	cbuffer& operator=(const cbuffer<Q> &other) {
		#ifndef NDEBUG
			std::cout<<"cbuffer::operator=(const cbuffer<Q> &other)"<<std::endl;
		#endif
		
		cbuffer<T> tmp(other);
		std::swap(cb,tmp.cb);
		std::swap(size,tmp.size);
		std::swap(items,tmp.items);		
		return *this;
	}
	
	/**
	 * metodo che ritorna la dimensione del buffer
	 * @return size dimensione del cbuffer
	*/
	size_type get_size(void) const {
		return this->size;
	}
	
	/**
	 * metodo che ritorna il numero di item presenti nel buffer
	 * @return items elementi presenti nel buffer
	*/
	size_type get_items(void) const {
		return this->items;
	}
	
	 /**
	  * Accesso in scrittura alla posizione index tramite [];
	  * posso scrivere solo nelle posizioni in cui è gia presente
	  * un dato.
	  * @param index posizione in cui scrivere
	  * @return &T riferimento alla cella index-esima
	  * @throw VoidPosition se provo a leggere da una posizione vuota
	  * @throw IndexOutOfBound se l'index è fuori dal range consentito
	 */
	T& operator[](size_type index) {
		if(index < size){
			if(index < items)
				return cb[index];
			else
				throw VoidPosition("impossibile scrivere ( operator [])");
		}
		else
			throw IndexOutOfBound("index non valido (operator [])");
	}
	
	/**
	 * Accesso in lettura alla posizione index tramite [];
	 * @param index posizione da cui leggere
	 * @return const &T riferimento costante alla posizione index-esima
	 * @throw VoidPosition se provo a leggere da una posizione vuota
	 * @throw IndexOutOfBound se l'index è fuori dal range consentito
	 */
	const T& operator[](size_type index) const{
		if(index < size){
			if(index < items)
				return cb[index];
			else
				throw VoidPosition("posizione vuota (operator [])");
		}
		else
			throw IndexOutOfBound("index non valido (operator [])");
	}
	
	/**
	 * metodo get che ritorna un valore specifico di posizione index nel cbuffer 
	 * (sola lettura)
	 * @param index poszione da cui leggere
	 * @return const &T riferimento costante alla posizione index-esima
	 * @throw VoidPosition se provo a leggere da una posizione vuota
	 * @throw IndexOutOfBound se l'index è fuori dal range consentito
	*/
	const T& get_cbuffer(const size_type index) const{
		if(index<size){
			if(index < items)
				return cb[index];
			else
				throw VoidPosition("posizione vuota (metodo get)");
		}
		else
			throw IndexOutOfBound("index non valido (mtodo get)");
	}

	/**
	 * metodo set per settare un valore specifo di poszizone index nel cbuffer;
	 * posso solo scrivere nelle posizioni in cui era già presente un dato
	 * @param index posizione del valore da settare
	 * @param const T &x riferimento costante al valore da inserire
	 * @throw VoidPosition se provo a leggere da una posizione vuota
	 * @throw IndexOutOfBound se l'index è fuori dal range consentito
	*/
	void set_cbuffer(size_type index, const T &x){
		if(index<size){
			if(index < items)
				cb[index]=x;
			else
				throw VoidPosition("impossibile scrivere (metodo set)");
		}
		else
			throw IndexOutOfBound("index non valido (metodo set)");
	}
	
	/**
	 * metodo value equivalente all'operatore [] const (lettura)
	 * @param index posizione da cui leggere
	 * @return const &T riferimento costante alla cella index-esima
	 * @throw VoidPosition se provo a leggere da una posizione vuota
	 * @throw IndexOutOfBound se l'index è fuori dal range consentito
	*/
	const T& value(size_type index) const{
		if(index<size){
			if(index < items)
				return cb[index];
			else
				throw VoidPosition("posizione vuota (metodo value)");
		}
		else 
			throw IndexOutOfBound("index non valido (metodo value)");
		
	}
	
	/**
	 * metodo value equivalente all'operatore [] senza const (scrittura)
	 * @param index posizione i cui scrivere
	 * @return &T riferimento alla cella index-esima
	 * @throw VoidPosition se provo a leggere da una posizione vuota
	 * @throw IndexOutOfBound se l'index è fuori dal range consentito
	*/
	T& value(size_type index){
		if(index<size){
			if(index < items)
				return cb[index];
			else
				throw VoidPosition("impossibile scrivere (metodo value)");
		}
		else 
			throw IndexOutOfBound("index non valido (metoo value)");
	}
	
	/**
	 * operazione di inserimento in coda: se il buffer è pieno cancello il più vecchio
	 * (il più vecchio diventerà cb[1]) e inserisco il nuovo elemento in coda;
	 * @param const T& riferimento costante al valore da inserire
	*/
	void insert(const T &x){
		assert(size>0);
		
		if (items==size){
			del();
		}
		cb[items]=x;
		items++;
	}
	
	
	/**
	 * operazione di cancellazione in testa: cancello l'elemento più vecchio cb[0] 
	 * appoggiandomi a una procedura privata per mantenere il pù vecchio in testa (cb[1])
	*/
	void del(){
		assert(items>0);
		//procedura d'appoggio privata
		shift_left();
		
	}
	
	class const_iterator; //forward declaration
	
	/**
	 * Forward iterator di lettura e scrittura
	*/
	class iterator {
		T *ptr; ///< elemento del buffer a cui punta l'iteratore
		
		/**
		 * Costruttore privato per inizializzare ptr;
		 * cbuffer può chiamarlo grazie alla friend
		 * @param p puntatore ai dati di cbuffer
		*/
		iterator(T* p) : ptr(p) {}
		
		friend class cbuffer; //permette a cbuffer di chiamare il costruttore privato

	public:
		/**
		 * Costruttoe i default
		*/
		iterator() : ptr(0) {}
		
		/**
		 * Copy construct
		 * @param other iterator da copiare.
		*/
		iterator(const iterator &other) : ptr(other.ptr) {}
		
		/**
		 * Operazione assegnamento
		 * @param other iterator da copiare
		*/
		iterator& operator=(const iterator &other) {
			ptr = other.ptr;
			return *this;		
		}
		
		/**
		 * distruttore
		*/
		~iterator() {}	
		
		/**
		 * Dereferenziamento 
		 * @return reference al dato puntato
		*/
		T& operator*() const {
			return *ptr;		
		}

		/**
		 * Puntatore
		 * @return puntatore al dato.
		*/
		T* operator->() const {
			return ptr;
		}
		
		/**
		 * Confronto tra iterator
		 * @param other iterator da confrontare
		 * @return true se *this e other puntano allo stesso elemento
		*/
		bool operator==(const iterator &other) const {
			return (ptr == other.ptr);	
		}
		
		/**
		 * Confronto tra iterator
		 * @param other iterator da confrontare
		 * @return true se *this e other non puntano allo stesso elemento
		*/
		bool operator!=(const iterator &other) const {
			return !(other==*this);		
		}
		
		/**
		 * Spostamento prefisso
		 * @return iteartor nella nuova posizione
		*/
		iterator& operator++() {
			++ptr;
			return *this;
		}
		
		/**
		 * Spostamento postfisso
		 * @return iterator nella vecchia posizione
		*/
		iterator operator++(int) {
			iterator tmp(ptr);
			++ptr;
			return tmp;		
		}
		
		friend class const_iterator;
		
		/**
		 * Operatore di confronto con const_iterator (uguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano allo stesso dato
		 */
		bool operator==(const const_iterator &other) const {
			return (ptr	== other.ptr);
		}

		/**
		 * Operatore di confronto con const_iterator (disuguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano a dati diversi
		 */
		bool operator!=(const const_iterator &other) const {
			return (ptr != other.ptr);
		}
		
	}; //fine classe iterator
	
	/**
	 * Iteratore di inizio: torna l'iteratore all'elemento più vecchio cb[0]
	 * @return iterator all'elemento più vecchio.
	*/
	iterator begin() {
		return iterator(cb);
	}
	
	/**
	 * iteratore di fine.
	 * @return iterator di fine.
	*/
	iterator end() {
		return iterator(cb+items);
	}
	
	/**
	 * Forward iterator di lettura 
	 */
	class const_iterator {
		T *ptr; ///< elemento del buffer a cui punta il const_iterator
		
		/**
		 * Costruttore privato per inizializzare ptr;
		 * cbuffer può chiamarlo grazie alla friend
		 * @param p puntatore ai dati di cbuffer
		*/
		const_iterator(T* p) : ptr(p) {}
		
		friend class cbuffer;
		
	public:
		/**
		 * Costruttore di default
		 */
		const_iterator() : ptr(0) {}
		
		/**
		 * Costruttore di copia
		 * @param other iteratore da copiare
		 */
		const_iterator(const const_iterator &other) : ptr(other.ptr) {}

		/**
		 * Operatore di assegnamento
		 * @param other iteratore da copiare
		 */
		const_iterator& operator=(const const_iterator &other) {
			ptr = other.ptr;
			return *this;
		}

		/**
		 * Distruttore
		 */
		~const_iterator() { }

		/**
		 * Dereferenziamento
		 * @return il valore contenuto nel nodo
		 */
		const T& operator*() const {
			return *ptr;
		}

		/**
		 * Dereferenziamento
		 * @return il puntatore al valore contenuto nel nodo
		 */
		const T* operator->() const {
			return ptr;
		}
		
		/**
		 * Elemento successivo nella sequenza (post-incremento)
		 * @return l'iteratore corrispondente all'elemento corrente 
		 */
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			ptr++;			
			return tmp;
		}

		/**
		 * Elemento successivo nella sequenza (pre-incremento)
		 * @return l'iteratore corrispondente all'elemento succesivo 
		 */
		const_iterator& operator++() {
			++ptr;
			return *this;
		}

		/**
		 * Operatore di confronto con const_iterator (uguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano allo stesso dato
		 */
		bool operator==(const const_iterator &other) const {
			return (ptr==other.ptr);
		}
		
		/**
		 * Operatore di confronto con const_iterator (disuguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano a dati diversi
		 */
		bool operator!=(const const_iterator &other) const {
			return (ptr!=other.ptr);
		}

		// Solo se serve anche iterator aggiungere le seguenti definizioni
		friend class iterator;

		/**
		 * Operatore di confronto con iterator (uguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano allo stesso dato
		 */
		bool operator==(const iterator &other) const {
			return (ptr==other.ptr);
		}

		/**
		 * Operatore di confronto con iterator (disuguaglianza)
		 * @param other iteratore da confrontare
		 * @return true se i due iteratori puntano a dati diversi
		 */
		bool operator!=(const iterator &other) const {
			return (ptr!=other.ptr);
		}
	
	}; // fine classe const_iterator
	
	/**
	 * Iteratore costante di inizio buffer
	 * @return l'iteratore costante di inizio buffer
	 */
	const_iterator begin() const {
		return const_iterator(cb);
	}
	
	/**
	 * Iteratore costante di fine buffer
	 * @return l'iteratore costante di fine buffer
	 */
	const_iterator end() const {
		return const_iterator(cb+items);
	}

};// fine classe cbuffer

/**
 * Operatore di stream di output per la stampa di un cbuffer
 * @param os stream di input
 * @param cb cbuffer da stampare
 * @return stream di output
*/
template <typename T>
std::ostream &operator<<(std::ostream &os,const cbuffer<T> &cb) {
	for(typename cbuffer<T>::size_type i=0;i<cb.get_items();++i)
		os <<  cb[i] << " ";

	return os;
}

/**
 * funzione globale check che, dati un cbuffer generico cb e un
 * predicato unario generico P, per ogni elemento cb[i] contenuto 
 * nel cbuffer stampa [i]: true se rispetta il predicato, [i]: false 
 * atrimenti
 * @param cb cbuffer da controllare
 * @param pred predicato unario generico
*/
template <typename T, typename P>
void check(const cbuffer<T> &cb, P pred){
	for(typename cbuffer<T>::size_type i=0;i<cb.get_items();i++){
		if(pred(cb[i])){
			std::cout << "[" << i << "]: true."<< std::endl;
		}
		else{
			std::cout << "[" << i << "]: false."<< std::endl;
		}
	}
}

#endif
