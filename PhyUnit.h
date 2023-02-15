#pragma once

#ifndef PHY_UNIT_DECL
#define PHY_UNIT_DECL

#include "pch.h"
#include "Engine.h"
#include <ostream>
#include <map>
#include <vector>

namespace Phy {

	enum class VISIONPX_API UnitPrefix {
		//I numeri rappresentano 1Ex dove x è il numero, quindi x=12 (TERA) => 1E12
		QUETTA = 30,
		RONNA = 27,
		YOTTA = 24,
		ZETTA = 21,
		EXA = 18,
		PETA = 15,
		TERA = 12,
		GIGA = 9,
		MEGA = 6,
		CHILO = 3,
		ETTO = 2,
		DECA = 1,
		DEFAULT = 0,
		DECI = -1,
		CENTI = -2,
		MILLI = -3,
		MICRO = -6,
		NANO = -9,
		PICO = -12,
		FEMTO = -15,
		ATTO = -18,
		ZEPTO = -21,
		YOCTO = -24,
		RONTO = -27,
		QUECTO = -28
	};

	class VISIONPX_API UnitComponent
	{
		friend class UnitComponentFactory;
		friend VISIONPX_API std::string UnitComponentToString(UnitComponent const* uc);
		friend VISIONPX_API std::ostream& operator<< (std::ostream& out, UnitComponent const& data);
		friend VISIONPX_API Phy::UnitComponent& operator<< (UnitComponent& out, UnitComponent const& data);
		friend VISIONPX_API Phy::UnitComponent& operator* (UnitComponent const& uc1, UnitComponent const& uc2);
		friend VISIONPX_API Phy::UnitComponent& operator/ (UnitComponent const& uc1, UnitComponent const& uc2);
		friend VISIONPX_API bool operator== (UnitComponent const& uc1, UnitComponent const& uc2);

	protected:
		/*
		Le varibili con flag _protected sono le variabili che definisco in questa libreria e sono quelle basi
		definite come globali nel metodo "VEngine::VEngineCore::init()".
		*/
		bool _protected;
		/*
		Le variabili con flag _cached sono quelle salvate in memoria ma che possono essere eliminate quando
		lo stack di cached raggiunge il massimo.
		*/
		bool _cahed;
		bool _global;
		bool _composed;
		std::string* name;
		std::string* symb;
		std::map<UnitComponent const*, double>* units;

	protected:
		/*
		Crea un'unità come valore costante. In questo modo la posso usare con gli operatori inline *, /, ^, ...
		*/
		const UnitComponent();
		/*
		Costruttore che permette di copiare l'oggetto.
		*/
		const UnitComponent(std::string* name, std::string* symb);
		const UnitComponent(std::string* name, std::string* symb, bool _protected);
		void setDefinition(std::string* name, std::string* symb);

	public:
		UnitComponent(UnitComponent const& uc);
		UnitComponent(UnitComponent const* uc);
		~UnitComponent();

		Phy::UnitComponent& operator^ (double power) const;//const: serve per abilitare l'uso del metodo anche sulle variabili const
		/*
		Aggiunge una nuova unità con il relativo esponente
		*/
		void addUnit(UnitComponent const* c, bool inverse);

		/*
		Controlla se contiene un'unità e ne ritorna il valore dell'esponente.
		*/
		double hasUnit(UnitComponent const* c);

		/*
		Una variabile protetta non può essere eliminata ed è considerata base: non
		possono essere eseguite operazioni di modifica su di essa.
		*/
		bool isProtected();

		/*
		Un'unità cached viene eliminata dopo un'utilizzo
		*/
		bool isCached();

		/*
		Un'unità cached viene eliminata dopo un'utilizzo
		*/
		void setCached(bool cached);

		/*
		Normalizza l'unità: tutte le unità composte vengono scomposte
		nelle loro componenti.
		Le globali non composte o protected NON vengono scomposte.
		*/
		UnitComponent const* normalize() const;

	private:
		void insertUnit(UnitComponent const* uc, double val);

		void pow(double val);

		void multiply(UnitComponent const* uc);

		void root(double index);

		void divide(UnitComponent const* uc);
	};

	/**
	Classe che permette di gestire le unità: la creazione e la distruzione e il salvataggio.
	Tramite questa classe ci si interfaccia alla classe Phy::UnitComponent.
	*/
	class VISIONPX_API UnitComponentFactory {
		friend VEngine::VEngineCore;
		friend class UnitComponent;
		friend VISIONPX_API Phy::UnitComponent& operator* (UnitComponent const& uc1, UnitComponent const& uc2);
		friend VISIONPX_API Phy::UnitComponent& operator/ (UnitComponent const& uc1, UnitComponent const& uc2);

	private:
		/*
		Mappa che contiene le Phy::UnitComponent globalizzate.
		sono salvate come: Nome unità => Pointer a unità

		Le unità globalizzate possono essere usate come unità standard per creare altre unità
		evitando di copiare ad ogni operazione il contenuto (le unità di cui è composto)
		*/
		std::map<std::string, UnitComponent const*>* global_units;
		/*
		Vettore che contiene le unità frutto di operazioni immediate come divisioni, moltiplicazioni, potenze.

		Queste unità ogni volta che vengono usate in calcoli vengono ricopiate tutte le unità di
		cui è composta nella nuova unità, a differenza di quelle globalizzate che vengono inserite come
		riferimento diretto.
		*/
		std::vector<UnitComponent const*>* cached_units;
		/*
		Indice dell'ultima unità salvata nella cache
		*/
		int last_cached_unit;
		/*
		Unica instanza della Factory. Uso pattern singleton.
		*/
		static UnitComponentFactory* instance;

		/*
		Costruttore privato perchè sto usando pattern singleton
		*/
		UnitComponentFactory();
		/*
		Rimuovo il costruttore che permette la copia di un oggetto
		in modo da avere un'unica instanza di questa classe.
		*/
		UnitComponentFactory(const UnitComponentFactory& uc) = delete;

	protected:
		/*
		Aggiunge alla cache un'unità. Le unità nella cache non sono permanenti e sono
		usate solo per derivare altre unità.

		Se ho più di (valore impostato in pch.h) unità nella cache
		sovrascrivi la più vecchia eliminandola.
		*/
		UnitComponent const* cacheUnit(UnitComponent const* unit);
		/*
		Data un'istanza Phy::UnitComponent creata dalla factory (è un puntatore costante) viene
		rimossa dalla cache e salvata nelle unità globali.

		Questo metodo è privato perchè è accessibile dai metodi "createUnit"; questi metodi inizializzano
		e salvano come globale una nuova unità salvata nella stack.
		*/
		UnitComponent const* globalizeUnit(UnitComponent const* unit);

	public:
		/*
		Devo rendere pubblico il distruttore altrimenti questa instanza
		non può essere eliminata
		*/
		~UnitComponentFactory();
		/*
		Singleton. Ritorna l'unica instanza di questa Factory del programma.
		*/
		static UnitComponentFactory* getInstance();

		/*
		Crea una nuova variabile globale con nome e simbolo. Le unità devono essere poi aggiunte.
		*/
		UnitComponent* createUnit(std::string* name, std::string* symb);
		/*
		Crea una nuova variabile globale con nome e simbolo. Le unità devono essere poi aggiunte.
		*/
		UnitComponent* createUnit(char** name, char** symb, int name_length, int symb_length);
		/*
		Crea una nuova variabile globale con nome e simbolo. Le unità devono essere poi aggiunte.
		*/
		UnitComponent* createUnit(const char* name, const char* symb);

		/*
		Crea un'unità cached: non vengono salvate in quelle globale e dopo un'utilizzo vengono eliminate.
		*/
		UnitComponent* compose(UnitComponent const* element1, UnitComponent const* element2);

		/*
		Imposta un'unità cached a non-cached e la salva nella lista delle globali.
		*/
		bool save(UnitComponent* uc);

		static const UnitComponent _1;
		static const UnitComponent METRO;
		static const UnitComponent SECONDO;
		static const UnitComponent AMPERE;
		static const UnitComponent GRAMMO;
		static const UnitComponent CANDELA;
		static const UnitComponent KELVIN;
		static const UnitComponent MOLE;
	};


	std::string VISIONPX_API UnitComponentToString(UnitComponent const* uc);


	class VISIONPX_API Unit {

	private:
		UnitPrefix prefix;
		UnitComponent const* unit_type;

	public:
		Unit();

		Unit(const Unit& c);

		Unit(UnitComponent const* u_type);

		Unit(UnitPrefix prx, UnitComponent const* u_type);

		void setPrefix(UnitPrefix prx);

		UnitPrefix getPrefix();

		UnitComponent const* getUnitType();
	};

	class VISIONPX_API UnitFactory {
		friend VEngine::VEngineCore;
		friend class UnitComponent;
		friend VISIONPX_API Phy::Unit& operator* (Unit const& uc1, Unit const& uc2);
		friend VISIONPX_API Phy::Unit& operator/ (Unit const& uc1, Unit const& uc2);

	public:
		static const Unit _1;
		static const Unit METRO;
		static const Unit SECONDO;
		static const Unit AMPERE;
		static const Unit GRAMMO;
		static const Unit CANDELA;
		static const Unit KELVIN;
		static const Unit MOLE;
	};

}

#endif // !PHY_UNIT_DECL