#include "pch.h"
#include "PhyUnit.h"
#include "utils.h"
#include <stdio.h>
#include <iostream>

Phy::UnitComponentFactory* Phy::UnitComponentFactory::instance = nullptr;
const Phy::UnitComponent Phy::UnitComponentFactory::_1(new std::string("Adimensionale"), new std::string(""), true);
const Phy::UnitComponent Phy::UnitComponentFactory::METRO(new std::string("Metro"), new std::string("m"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::SECONDO(new std::string("Secondo"), new std::string("s"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::AMPERE(new std::string("Ampere"), new std::string("A"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::GRAMMO(new std::string("Grammo"), new std::string("g"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::CANDELA(new std::string("Candela"), new std::string("c"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::KELVIN(new std::string("Kelvin"), new std::string("K"), true);
const Phy::UnitComponent Phy::UnitComponentFactory::MOLE(new std::string("Mole"), new std::string("mol"), true);

const Phy::Unit Phy::UnitFactory::_1(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::_1);
const Phy::Unit Phy::UnitFactory::AMPERE(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::AMPERE);
const Phy::Unit Phy::UnitFactory::METRO(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::METRO);
const Phy::Unit Phy::UnitFactory::SECONDO(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::SECONDO);
const Phy::Unit Phy::UnitFactory::GRAMMO(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::GRAMMO);
const Phy::Unit Phy::UnitFactory::CANDELA(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::CANDELA);
const Phy::Unit Phy::UnitFactory::KELVIN(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::KELVIN);
const Phy::Unit Phy::UnitFactory::MOLE(Phy::UnitPrefix::DEFAULT, &Phy::UnitComponentFactory::MOLE);

Phy::UnitComponent::UnitComponent() {
	this->units = new std::map<UnitComponent const*, double>();
	this->name = new std::string("_tmp");
	this->symb = new std::string("#");
	this->_protected = false;
	this->_cahed = false;
	this->_global = false;
	this->_composed = false;
}

Phy::UnitComponent::UnitComponent(UnitComponent const& uc) {
	this->units = new std::map<UnitComponent const*, double>();
	this->name = new std::string(*uc.name);
	this->name->append("#derived");
	this->symb = new std::string(*uc.symb);
	this->units->insert(uc.units->begin(), uc.units->end());
	this->_protected = uc._protected;
	this->_cahed = false;
	this->_global = false;
	this->_composed = false;
	this->_composed = false;
}

Phy::UnitComponent::UnitComponent(UnitComponent const* uc) {
	this->units = new std::map<UnitComponent const*, double>();
	this->name = new std::string(*uc->name);
	this->name->append("#derived");
	this->symb = new std::string(*uc->symb);
	this->units->insert(uc->units->begin(), uc->units->end());
	this->_protected = uc->_protected;
	this->_cahed = false;
	this->_global = false;
	this->_composed = false;
}

Phy::UnitComponent::UnitComponent(std::string* name, std::string* symb) {
	this->units = new std::map<UnitComponent const*, double>();
	this->name = new std::string(*name);
	this->symb = new std::string(*symb);
	this->_protected = false;
	this->_cahed = false;
	this->_global = false;
	this->_composed = false;
}

Phy::UnitComponent::UnitComponent(std::string* name, std::string* symb, bool _protected) {
	this->units = new std::map<UnitComponent const*, double>();
	this->name = new std::string(*name);
	this->symb = new std::string(*symb);
	this->_protected = _protected;
	this->_cahed = _protected;
	this->_global = false;
	this->_composed = false;
}

Phy::UnitComponent::~UnitComponent() {
	if (this->name) {
		delete this->name;
	}
	if (this->symb) {
		delete this->symb;
	}
	delete this->units;
}

void Phy::UnitComponent::setDefinition(std::string* name, std::string* symb)
{
	this->name = new std::string(*name);
	this->symb = new std::string(*symb);
}

bool Phy::UnitComponent::isProtected()
{
	return this->_protected;
}

void Phy::UnitComponent::insertUnit(UnitComponent const* uc, double val) {
	std::map<UnitComponent const*, double>::iterator iter = this->units->find(uc);
	this->_composed = true;
	if (iter == this->units->end()) {
		//std::cout << "Non trovato" << std::endl;
		this->units->insert(std::pair<UnitComponent const*, double>(uc, val));
	}
	else {
		double value = this->units->at(uc);
		value += val;
		iter->second = value;
		//std::cout << "Trovato e aggiornato: " << value << std::endl;
	}
}

/*
Aggiunge un'unità a questa con il relativo esponente (o nel caso sia già presente somma gli esponenti)
*/
void Phy::UnitComponent::addUnit(UnitComponent const* c, bool inverse) {
	if (c->_global) {//se è salvata come globale aggiungi direttamente l'unità
		this->insertUnit(c, inverse ? -1.0 : 1.0);
	}
	else {//altrimenti se non è salvata in memoria copio le unità
		std::map<UnitComponent const*, double>::iterator iter = c->units->begin();
		if (inverse) {//Li ho divisi per ottimizzare
			while (iter != c->units->end()) {
				this->insertUnit(iter->first, -(iter->second));
				iter++;
			}
		}
		else {
			while (iter != c->units->end()) {
				this->insertUnit(iter->first, iter->second);
				iter++;
			}
		}
	}
}

Phy::UnitComponent const* Phy::UnitComponent::normalize() const {
	UnitComponent* n_comp = new UnitComponent();
	n_comp->setCached(true);
	std::map<UnitComponent const*, double>::iterator iter = this->units->begin();
	while (iter != this->units->end()) {
		UnitComponent const* uc = iter->first;
		if ((uc->_global && !uc->_composed) || uc->_protected) {
			//TODO:controllare se già è presente e aggiornare il valore in caso
			std::map<UnitComponent const*, double>::iterator _sub_iter = n_comp->units->find(uc);
			if (n_comp->units->end() == _sub_iter) {//Non è ancora presente
				n_comp->units->insert(std::pair< UnitComponent const*, double>(uc, iter->second));
			}
			else {//è già stato aggiunto, modifica l'esponente
				_sub_iter->second = _sub_iter->second + iter->second;
			}
		}
		else if (uc->_composed) {
			//TODO:controllare se già è presente e aggiornare il valore in caso
			//Normalizzo l'unità 
			UnitComponent const* _units_to_add = uc->normalize();
			//Devo considerare anche l'esponente di quello composto
			double exp_units_to_add = iter->second;
			//Prendo le unità e le ciclo una a una
			std::map<UnitComponent const*, double>::iterator _sub_iter = _units_to_add->units->begin();
			while (_sub_iter != _units_to_add->units->end()) {
				//Controlla se è già presente l'unità nell'unità principale
				std::map<UnitComponent const*, double>::iterator _found_iter = n_comp->units->find(uc);
				if (n_comp->units->end() == _found_iter) {//Non è ancora presente
					n_comp->units->insert(std::pair< UnitComponent const*, double>(_sub_iter->first, (_sub_iter->second) * exp_units_to_add));
				}
				else {//è già stato aggiunto, modifica l'esponente
					_found_iter->second = (_sub_iter->second) * exp_units_to_add + iter->second;
				}
				_sub_iter++;
			}
		}
		else {//Non composta, singolare
			//TODO:controllare se già è presente e aggiornare il valore in caso
			std::map<UnitComponent const*, double>::iterator _sub_iter = n_comp->units->find(uc);
			if (n_comp->units->end() == _sub_iter) {//Non è ancora presente
				n_comp->units->insert(std::pair< UnitComponent const*, double>(uc, iter->second));
			}
			else {//è già stato aggiunto, modifica l'esponente
				_sub_iter->second = _sub_iter->second + iter->second;
			}
		}
		iter++;
	}
	return n_comp;
}

/*
Esegue l'operazione di potenza sulle unità di misura presenti in quest'unità:
moltiplica tutti gli esponenti delle unità per il valore della potenza.
*/
void Phy::UnitComponent::pow(double val)
{
	std::map<UnitComponent const*, double>::iterator iter;
	for (iter = this->units->begin(); iter != this->units->end(); iter++) {
		iter->second = iter->second * val;
	}
}

void Phy::UnitComponent::multiply(UnitComponent const* uc) {
	std::map<UnitComponent const*, double>::iterator iter = uc->units->begin();
	while (iter != uc->units->end()) {
		this->insertUnit(iter->first, iter->second);
		iter++;
	}
}

void Phy::UnitComponent::root(double index)
{
	std::map<UnitComponent const*, double>::iterator iter;
	for (iter = this->units->begin(); iter != this->units->end(); iter++) {
		iter->second = iter->second / index;
	}
}

void Phy::UnitComponent::divide(UnitComponent const* uc)
{
	std::map<UnitComponent const*, double>::iterator iter = uc->units->begin();
	while (iter != uc->units->end()) {
		this->insertUnit(iter->first, -iter->second);
		iter++;
	}
}

/*
Controlla se questa unità contiene un'altra unità.
@return Valore dell'esponente oppure 0 se non contiene l'unità cercata.
*/
double Phy::UnitComponent::hasUnit(UnitComponent const* c) {
	std::map<UnitComponent const*, double>::iterator iter = this->units->find(c);
	if (iter == this->units->end()) {//Non trova corrispondenze
		return 0;
	}
	return iter->second;//Trova la corrispondenza e ritorna il valore dell'esponente
}

/*
Nuova instanza della Factory delle unità.
*/
Phy::UnitComponentFactory::UnitComponentFactory() {
	this->global_units = new std::map<std::string, UnitComponent const*>();
	this->cached_units = new std::vector<UnitComponent const*>(MAX_UNITS_CACHED_DIMENSION, nullptr);
	this->last_cached_unit = 0;
}

/*
Distruttore dell'instanza della Factory.
Elimina tutte reference alle stringhe che rappresentano i nomi delle unità
e tutte le UnitComponent create.
*/
Phy::UnitComponentFactory::~UnitComponentFactory()
{
	std::map<std::string, UnitComponent const*>::iterator iter = this->global_units->begin();
	while (iter != this->global_units->end()) {
		delete& (iter->first);//Elimina la stringa con riferimento
		delete iter->second;//Elimina l'instanza UnitComponent per il puntatore corrente
	}
	this->global_units->clear();//Pulisce la mappa da tutti gli elementi
	delete this->global_units;//Elimina l'instanza della mappa
}

/*
Ritorna l'unica instanza creata.
*/
Phy::UnitComponentFactory* Phy::UnitComponentFactory::getInstance() {
	if (Phy::UnitComponentFactory::instance == nullptr) {
		Phy::UnitComponentFactory::instance = new Phy::UnitComponentFactory();
	}
	return Phy::UnitComponentFactory::instance;
}

/*
Crea una nuova unità con nome e simbolo utilizzando reference a stringhe.
*/
Phy::UnitComponent* Phy::UnitComponentFactory::createUnit(std::string* name, std::string* symb) {
	if (name != nullptr && symb != nullptr && name->size() < 100 && symb->size() < 10) {
		if (this->global_units->find(*name) == this->global_units->end()) {//Controllo se non esiste un'unità con lo stesso nome
			UnitComponent* uc = new UnitComponent(name, symb);//Creo nella stack la nuova unit
			this->global_units->insert(std::pair<std::string, UnitComponent const*>(*name, uc));//La inserisco nella mappa
			uc->_global = true;
			return uc;
		}
		std::string error = "Unit \"";
		error += *name;
		error += "\" alredy defined";
		throw error.c_str();//Unità già definita (stesso nome)
	}
	throw "Name(max 99 chars) or Symb(max 9 chars) not defined correctly";//Parametri non corretti
}

/*
Crea una nuova unità con nome e simbolo utilizzando array di caratteri per poi creare stringhe.
*/
Phy::UnitComponent* Phy::UnitComponentFactory::createUnit(char** name, char** symb, int name_length, int symb_length) {
	if (name != nullptr && symb != nullptr && name_length < 100 && symb_length < 10) {
		std::string* nm = new std::string();//Creo stringa per nome
		nm->copy(name[0], name_length);//Copio i caratteri del nome
		std::string* sym = new std::string();//Creo stringa per il simbolo
		sym->copy(symb[0], symb_length);//Copio i caratteri del simbolo
		if (this->global_units->find(*nm) == this->global_units->end()) {//Controllo se non esiste un'unità con lo stesso nome
			UnitComponent* uc = new UnitComponent(nm, sym);//Creo nella stack la nuova unità
			this->global_units->insert(std::pair<std::string, UnitComponent const*>(*nm, uc));//La inserisco nella mappa
			uc->_global = true;
			return uc;
		}
		std::string error = "Unit \"";
		error += *nm;
		error += "\" alredy defined";
		throw error.c_str();//Unità già definita (stesso nome)
	}
	throw "Name(max 99 chars) or Symb(max 9 chars) not defined correctly";//Parametri non corretti
}

Phy::UnitComponent* Phy::UnitComponentFactory::createUnit(const char* name, const char* symb) {
	if (name != nullptr && symb != nullptr) {
		std::string* nm = new std::string();//Creo stringa per nome
		std::string* sym = new std::string();//Creo stringa per il simbolo
		Utils::stringcopy(nm, name);//Copio i caratteri del nome
		Utils::stringcopy(sym, symb);//Copio i caratteri del simbolo
		if (this->global_units->find(*nm) == this->global_units->end()) {//Controllo se non esiste un'unità con lo stesso nome
			UnitComponent* uc = new UnitComponent(nm, sym);//Creo nella stack la nuova unità
			this->global_units->insert(std::pair<std::string, UnitComponent const*>(*nm, uc));//La inserisco nella mappa
			uc->_global = true;
			return uc;
		}
		std::string error = "Unit \"";
		error += *nm;
		error += "\" alredy defined";
		throw error.c_str();//Unità già definita (stesso nome)
	}
	throw "Name(max 99 chars) or Symb(max 9 chars) not defined correctly";//Parametri non corretti
}

Phy::UnitComponent const* Phy::UnitComponentFactory::cacheUnit(UnitComponent const* unit) {
	if (!unit->_global && !unit->_protected) {//Controllo che non sia flaggata come globale o protetta
		//faccio il cast a pointer non costante
		//Altrimenti non potrei modificarla
		UnitComponent* uc = const_cast<Phy::UnitComponent*>(unit);
		uc->_cahed = true;//la flaggo come cached
		if (this->last_cached_unit > MAX_UNITS_CACHED_DIMENSION) {//controllo se sto uscendo dal massimo della cache
			this->last_cached_unit = 0;
		}
		UnitComponent const* last_ptr = this->cached_units->at(this->last_cached_unit);//Prendo l'unità (se esiste) all'ultima posizione conosciuta
		if (last_ptr != nullptr) {//Se è presente un'unità cache (apparentemente ho riniziato il giro del vettore)
			delete last_ptr;//Eliminala
		}
		this->cached_units->insert(this->cached_units->begin() + this->last_cached_unit, uc);//E sostituisci o inserisci a questa posizione il nuovo valore cached
		this->last_cached_unit++;//Incremento l'indice
		return uc;
	}
	return nullptr;
}

Phy::UnitComponent const* Phy::UnitComponentFactory::globalizeUnit(UnitComponent const* unit) {
	UnitComponent* uc = const_cast<Phy::UnitComponent*>(unit);//Faccio il cast a puntatore normale
	uc->_global = true;//Flaggo l'unità come globale
	this->global_units->insert(std::pair<std::string, Phy::UnitComponent const*>(*unit->name, unit));//La aggiungo nel registro
	if (uc->_cahed) {//Se la variabile era anche cached allora la rimuovo da quel registro
		size_t index = Utils::findAt(this->cached_units, unit);//Trova l'indice in quella cached
		uc->_cahed = false;//Togli il flag cached
		if (index > -1) {//è presente nel registro?
			//Se è presente eliminala
			this->cached_units->insert(this->cached_units->begin() + index, nullptr);
		}
	}
	return uc;//ritorno lo stesso puntatore
}

Phy::UnitComponent* Phy::UnitComponentFactory::compose(UnitComponent const* element1, UnitComponent const* element2)
{
	UnitComponent* base = createUnit("_tmp#composed", "#composed#");//Crea una variabile composta
	//Aggiungi le due unità
	base->addUnit(element1, false);
	base->addUnit(element2, false);
	if (!element1->_protected && element1->_cahed) {//Unità non protetta e flaggata cached
		delete element1;//Allora dopo l'utilizzo la elimino
	}
	if (!element2->_protected && element2->_cahed) {//Unità non protetta e flaggata cached
		delete element2;//Allora dopo l'utilizzo la elimino
	}
	return base;
}

bool Phy::UnitComponentFactory::save(UnitComponent* uc)
{
	std::map<std::string, UnitComponent const*>::iterator iter = this->global_units->begin();
	while (iter != this->global_units->end()) {//Controlla se l'unità (il puntatore all'unità) è già stato salvato nelle globali
		if (iter->second == uc) {
			return false;//Se è già stato salvato nelle globali torno "false"
		}
	}
	uc->_cahed = false;//Altrimenti la imposto come variabile non "cached"
	this->global_units->insert(std::pair<std::string, UnitComponent const*>(*uc->name, uc)); //e la inserisco nelle globali
	return true;
}

inline std::ostream& Phy::operator<<(std::ostream& out, Phy::UnitComponent const& data)
{
	out << UnitComponentToString(&data); //Stampo la rappresentazione a stringa
	return out;
}

inline Phy::UnitComponent& Phy::operator<<(Phy::UnitComponent& out, Phy::UnitComponent const& data)
{
	out.addUnit(&data, false);
	return out;
}

inline Phy::UnitComponent& Phy::UnitComponent::operator^(double power) const
{
	Phy::UnitComponent* c = new Phy::UnitComponent();//Creo una nuova unità (puntatore costante)
	c->addUnit(this, false);//Aggiungo l'unità su cui sto eseguendo la potenza
	c->pow(power);//sommo a tutti gli esponenti della nuova unità 
	Phy::UnitComponentFactory::getInstance()->cacheUnit(c);//imposto la variabile come cached
	return *c;
}

inline Phy::UnitComponent& Phy::operator*(UnitComponent const& uc1, UnitComponent const& uc2)
{
	Phy::UnitComponent* c = new Phy::UnitComponent();//Creo una nuova unità (puntatore costante)
	//Aggiungo le due unità
	c->addUnit(&uc1, false);
	c->addUnit(&uc2, false);
	//Imposto la variabile come cached
	Phy::UnitComponentFactory::getInstance()->cacheUnit(c);
	return *c;
}

inline Phy::UnitComponent& Phy::operator/(UnitComponent const& uc1, UnitComponent const& uc2)
{
	//Creo una nuova unità unità
	Phy::UnitComponent* c = new Phy::UnitComponent();
	//Aggiungo l'unità 1
	c->addUnit(&uc1, false);
	//Aggiungo l'inverso dell'unità 2
	c->addUnit(&uc2, true);
	//Salvo la variabile nella cache
	Phy::UnitComponentFactory::instance->cacheUnit(c);
	return *c;
}

//TODO: devo modificare per fare in modo di normalizzare le unità: se contiene unità globalizzate, le devo de-globalizzare.
inline bool Phy::operator==(UnitComponent const& uc1, UnitComponent const& uc2)
{
	std::map<UnitComponent const*, double>::iterator iter = uc1.units->begin();
	//Controllo se ad ogni unità della prima corrisponde la stessa unità con lo stesso esponente nella seconda
	while (iter != uc1.units->end()) {
		std::map<UnitComponent const*, double>::iterator iter2 = uc2.units->find(iter->first);
		if (iter2 == uc2.units->end() || iter2->second != iter->second) {
			return false;
		}
		iter++;
	}
	return  true;
}

bool Phy::UnitComponent::isCached()
{
	return this->_cahed;
}

void Phy::UnitComponent::setCached(bool cached) {
	this->_cahed = cached;
}

std::string Phy::UnitComponentToString(UnitComponent const* uc)
{
	std::string out = *uc->name + " [" + *uc->symb;
	std::map<Phy::UnitComponent const*, double>::iterator iter = uc->units->begin();
	if (uc->units->size() > 0) {
		out += " = ";
		bool _p = true;
		while (iter != uc->units->end()) {
			if (_p) {
				_p = false;
			}
			else {
				out += "*";
			}
			out += *(iter->first->symb) + "^";
			out += std::to_string(iter->second);
			iter++;
		}
	}
	out += "]";

	return out;
}

Phy::Unit::Unit()
{
	this->prefix = UnitPrefix::DEFAULT;
	this->unit_type = &UnitComponentFactory::_1;
}

Phy::Unit::Unit(const Unit& c)
{
	this->prefix = c.prefix;
	this->unit_type = c.unit_type;
}

Phy::Unit::Unit(UnitComponent const* u_type)
{
	if (u_type && u_type != nullptr) {
		this->prefix = UnitPrefix::DEFAULT;
		this->unit_type = u_type;
	}
	else {
		throw "Null unit component not accepted";
	}
}

Phy::Unit::Unit(UnitPrefix prx, UnitComponent const* u_type)
{
	if (u_type && u_type != nullptr) {
		this->prefix = prx;
		this->unit_type = u_type;
	}
	else {
		throw "Null unit component not accepted";
	}
}

void Phy::Unit::setPrefix(UnitPrefix prx)
{
	this->prefix = prx;
}

Phy::UnitPrefix Phy::Unit::getPrefix()
{
	return this->prefix;
}

Phy::UnitComponent const* Phy::Unit::getUnitType()
{
	return this->unit_type;
}
