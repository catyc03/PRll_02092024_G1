#include <iostream> 
#include <string> 
#include <regex> 
#include <mutex> 
#include <thread> 
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Drzava { ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA, BOSNA_I_HERCEGOVINA };
ostream& operator<<(ostream& os, Drzava d) {
	switch (d)
	{
	case ENGLESKA:
		cout << "ENGLESKA";
		break;
	case SPANIJA:
		cout << "SPANIJA";
		break;
	case HOLANDIJA:
		cout << "HOLANDIJA";
		break;
	case FRANCUSKA:
		cout << "FRANCUSKA";
		break;
	case BOSNA_I_HERCEGOVINA:
		cout << "BOSNA_I_HERCEGOVINA";
		break;
	default:
		cout << "ERROR";
		break;
	}
	return os;
}
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}
char* GenerisiID(string imePrezime, int broj) {
	char ime = toupper(imePrezime[0]);
	char prezime = toupper(imePrezime[imePrezime.find(" ") + 1]);
	string Broj = to_string(broj);
	reverse(Broj.begin(), Broj.end());
	string nule = "";
	if (broj < 10)
		nule += "000";
	else if (broj < 100)
		nule += "00";
	else if (broj < 1000)
		nule += "0";
	string id = ime + nule + prezime + Broj;
	return GetNizKaraktera(id.c_str());
}
bool ValidirajID(string id) {
	regex oneDigit   ("[A-Z]000[A-Z][0-9]");
	regex twoDigit   ("[A-Z]00[A-Z][0-9]{2}");
	regex threeDigit ("[A-Z]0[A-Z][0-9]{3}");
	regex fourDigit  ("[A-Z][A-Z][0-9]{4}");
	return regex_match(id, oneDigit) ||
		   regex_match(id, twoDigit) ||
		   regex_match(id, threeDigit) ||
		   regex_match(id, fourDigit);
}

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}
	~Kolekcija() {
		delete _trenutno; _trenutno = nullptr;
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < getTrenutno(); i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this == &obj) {
			return *this;
		}
		delete _trenutno;
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < getTrenutno(); i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	void AddElement(T1 el1,T2 el2) {
		if (getTrenutno() == max)
			throw exception("Dupliranje XXX");

		_elementi1[getTrenutno()] = el1;
		_elementi2[getTrenutno()] = el2;
		(*_trenutno)++;
	}
	Kolekcija InsertAt(T1 el1, T2 el2,int at) {
		Kolekcija nova;
		for (int i = 0; i < getTrenutno(); i++) {
			if (i == at) {
				nova.AddElement(el1, el2);
			}
			nova.AddElement(getElement1(i), getElement2(i));
		}
		*this = nova;
		return *this;
	}
	void RemoveAt(int at) {
		Kolekcija nova;
		for (int i = 0; i < getTrenutno(); i++) {
			if (i == at) {
				continue;
			}
			nova.AddElement(getElement1(i), getElement2(i));
		}
		*this = nova;
	}
	Kolekcija RemoveRange(int from,int to) {
		Kolekcija nova;
		for (int i = from; i <= to; i++) {
			nova.AddElement(getElement1(from), getElement2(from));
			RemoveAt(from);
		}
		return nova;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
};
class Vrijeme {
	int* _sat, * _minuta, * _sekunda;
public:
	Vrijeme(int sat = 10, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}
	~Vrijeme() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
	Vrijeme(const Vrijeme& obj) {
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
	}
	Vrijeme& operator=(const Vrijeme& obj) {
		if (this == &obj) {
			return *this;
		}
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
		return *this;
	}
	int toSec() {
		return *_sekunda + *_minuta * 60 + *_sat * 3600;
	}
	bool operator==(const Vrijeme obj) {
		return *_sekunda == *obj._sekunda && *_minuta == *obj._minuta && *_sat == *obj._sat;
	}
	bool operator!=(const Vrijeme obj) {
		return !(*this == obj);
	}
	bool operator<(Vrijeme v) {
		return this->toSec() < v.toSec();
	}
	bool operator>(Vrijeme v) {
		return this->toSec() > v.toSec();
	}
	bool operator<=(Vrijeme v) {
		return this->toSec() <= v.toSec();
	}
	bool operator>=(Vrijeme v) {
		return this->toSec() >= v.toSec();
	}

	friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" << *obj._sekunda;
		return COUT;
	}
};
class Pogodak {
	Vrijeme* _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) {
		_napomena = GetNizKaraktera(napomena);
		_vrijemePogotka = new Vrijeme(vrijeme);
	}
	~Pogodak() {
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemePogotka; _vrijemePogotka = nullptr;
	}
	Pogodak(const Pogodak& obj) {
		_vrijemePogotka = new Vrijeme(*obj._vrijemePogotka);
		_napomena = GetNizKaraktera(obj._napomena);
	}
	Pogodak&operator=(const Pogodak& obj) {
		if (this == &obj) {
			return *this;
		}
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemePogotka;
		_vrijemePogotka = new Vrijeme(*obj._vrijemePogotka);
		_napomena = GetNizKaraktera(obj._napomena);
		return *this;
	}
	friend bool operator==(const Pogodak& p1, const Pogodak& p2) {
		if (*p1._vrijemePogotka != *p2._vrijemePogotka) return false;
		if (strcmp(p1._napomena, p2._napomena) != 0) return false;
		return true;
	}
	friend bool operator!=(const Pogodak& p1, const Pogodak& p2) {
		return !(p1 == p2);
	}
	Vrijeme GetVrijemePogotka() { return *_vrijemePogotka; }
	char* GetNapomena() { return _napomena; }
	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << *obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}
};

class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID tj.atributa _imePrezime(inicijala imena i prezimena) istatickog clana _id
	char* _imePrezime;
	vector<Pogodak*> _pogoci;
public:
	Igrac(const char* imePrezime) {
		_ID = GenerisiID(imePrezime, _id++);
		_imePrezime = GetNizKaraktera(imePrezime);
	}
	~Igrac() {
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _pogoci.size(); i++) {
			delete _pogoci[i];
			_pogoci[i] = nullptr;
		}
	}
	Igrac(const Igrac& obj) {
		_ID = GetNizKaraktera(obj._ID);
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		for (int i = 0; i < obj._pogoci.size(); i++) {
			_pogoci.push_back(new Pogodak(*obj._pogoci[i]));
		}
	}
	Igrac&operator=(const Igrac& obj) {
		if (this == &obj) {
			return *this;
		}
		_pogoci.clear();
		delete[]_ID; 
		delete[]_imePrezime;
		for (size_t i = 0; i < _pogoci.size(); i++) {
			delete _pogoci[i];
		}
		_ID = GetNizKaraktera(obj._ID);
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		for (int i = 0; i < obj._pogoci.size(); i++) {
			_pogoci.push_back(new Pogodak(*obj._pogoci[i]));
		}
		return *this;
	}
	
	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	vector<Pogodak*>& GetPogoci() { return _pogoci; }
	friend ostream& operator<< (ostream& COUT, Igrac& obj) {
		COUT << *obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			cout << *obj._pogoci[i] << endl;
		return COUT;
	}
};

int Igrac::_id = 1;
class Reprezentacija {
	Drzava _drzava;
	vector<Igrac> _igraci;
public:
	Reprezentacija(Drzava drzava = BOSNA_I_HERCEGOVINA) {
		_drzava = drzava;
	}
	~Reprezentacija(){}
	Reprezentacija(const Reprezentacija&obj){
		_drzava = obj._drzava;
		_igraci = obj._igraci;
	}
	Reprezentacija&operator=(const Reprezentacija&obj){
		if (this == &obj) {
			return *this;
		}
		_drzava = obj._drzava;
		_igraci = obj._igraci;
		return *this;
	}
	int getUkupnoGolova() {
		int br = 0;
		for (int i = 0; i < _igraci.size(); i++) {
			br += _igraci[i].GetPogoci().size();
		}
		return br;
	}
	vector<Igrac> getScorers() {
		vector<Igrac> igrac;
		for (int i = 0; i < _igraci.size(); i++) {
			for (int j = 0; j < _igraci[i].GetPogoci().size(); j++) {
				igrac.push_back(_igraci[i]);
			}
		}
		return igrac;
	}
	bool imaGO(Pogodak pogodak) {
		for (int i = 0; i < _igraci.size(); i++) {
			for (int j = 0; j < _igraci[i].GetPogoci().size(); j++) {
				if (*_igraci[i].GetPogoci()[j] == pogodak)
					return true;
			}
		}
		return false;
	}
	Igrac* getIgracByIDorName(string imeID) {
		for (int i = 0; i < _igraci.size(); i++) {
			if (imeID == _igraci[i].GetID() || imeID == _igraci[i].GetImePrezime())
				return &_igraci[i];
		}
		return nullptr;
	}
	void AddIgrac(Igrac igrac) { 
		for (int i = 0; i < _igraci.size(); i++) {
			if (strcmp(_igraci[i].GetID(), igrac.GetID())==0)
				throw exception("Igrac vec igra\n");
		}
		_igraci.push_back(igrac);
	}
	bool operator==(Reprezentacija r) {
		return _drzava == r.GetDrzava();
	}

	Drzava  GetDrzava() { return _drzava; }
	vector<Igrac>& GetIgraci() { return _igraci; }
};
class Prventstvo {
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
	mutex m;
	void mail(Reprezentacija rep1,Reprezentacija rep2,Pogodak pogodak,Igrac igrac) {
		m.lock();
		cout << "\nTo: " << igrac.GetID() << "@euro2024.com";
		cout << "From: info@euro2024.com\nSubject: Informacija\n\nPostovani,\n\n";
		cout << "U " << pogodak.GetVrijemePogotka() << " sati igrac " << igrac.GetImePrezime() << " je zabiljezio svoj " << igrac.GetPogoci().size() << " pogodak na ovoj utakmici.\n";
		cout << "Trenutni rezultat je: \n\n";
		cout << rep1.GetDrzava() << " " << rep1.getUkupnoGolova() << " : " << rep2.getUkupnoGolova() << " " << rep2.GetDrzava() << endl << endl;
		cout << "Puno srece u nastavku susreta.\nNeka bolji tim pobijedi." << endl;
		m.unlock();
	}
public:
	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() {
		return _utakmice;
	}
	Prventstvo() {}
	~Prventstvo() {}
	Prventstvo(const Prventstvo& obj) {
		_utakmice = obj._utakmice;
	}
	Prventstvo& operator=(const Prventstvo& obj) {
		if (this == &obj) {
			return *this;
		}
		_utakmice = obj._utakmice;
		return *this;
	}
	void AddUtakmicu(Reprezentacija rep1, Reprezentacija rep2) {
		for (size_t i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if (_utakmice.getElement1(i).GetDrzava() == rep1.GetDrzava() && _utakmice.getElement2(i).GetDrzava() == rep2.GetDrzava() ||
				_utakmice.getElement1(i).GetDrzava() == rep2.GetDrzava() && _utakmice.getElement2(i).GetDrzava() == rep1.GetDrzava())
				throw exception("Utakmica je odigrana\n");
		}
		_utakmice.AddElement(rep1, rep2);
	}
	bool postojiPogodak(Drzava d1, Drzava d2,Pogodak pogodak) {
		for (int i = 0; i < _utakmice.getTrenutno(); i++) {
			auto rep1 = _utakmice.getElement1(i);
			auto rep2 = _utakmice.getElement2(i);

			if (rep1.GetDrzava() == d1 && rep2.GetDrzava() == d2 || rep1.GetDrzava() == d2 && rep2.GetDrzava() == d1) {
				if (rep1.imaGO(pogodak) || rep2.imaGO(pogodak))
					return true;
			}
		}
		return false;
	}
	bool AddPogodak(Drzava d1,Drzava d2,const char* imeID,Pogodak pogodak) {
		if (postojiPogodak(d1, d2, pogodak))
			return false;
		for (int i = 0; i < _utakmice.getTrenutno(); i++) {
			auto& rep1 = _utakmice.getElement1(i);
			auto& rep2 = _utakmice.getElement2(i);
			if (rep1.GetDrzava() == d1 && rep2.GetDrzava() == d2 || rep1.GetDrzava() == d2 && rep2.GetDrzava() == d1) {
				Igrac* igrac = rep1.getIgracByIDorName(imeID);
				if (!igrac) igrac = rep2.getIgracByIDorName(imeID);
				if (!igrac) return false;
				igrac->GetPogoci().push_back(new Pogodak(pogodak));
				cout << crt;
				thread t(&Prventstvo::mail, this, rep1, rep2, pogodak, *igrac);
				t.join();
				return true;
			}
		}

		return false;
	}
	/*
	-------------------------------------------
	(3) BOSNA_I_HERCEGOVINA : ENGLESKA (1)
	-------------------------------------------
	Denis Music             Goran Skondric
	Jasmin Azemovic
	Jasmin Azemovic
	-------------------------------------------
	*/
	friend ostream& operator<<(ostream& COUT, Prventstvo& p) {
		for (int i = 0;i< p._utakmice.getTrenutno(); i++) {
			auto& rep1 = p._utakmice.getElement1(i);
			auto& rep2 = p._utakmice.getElement2(i);
			auto scorers1 = rep1.getScorers();
			auto scorers2 = rep2.getScorers();
			int brojIgraca = max(scorers1.size(), scorers2.size());

			COUT << crt << "(" << rep1.getUkupnoGolova() << ") " << rep1.GetDrzava() << " : " << rep2.GetDrzava() << " ("<<rep2.getUkupnoGolova() << ")" << crt;

			for (int j = 0; j < brojIgraca; j++) {
				if (j < scorers1.size()) {
					COUT << scorers1[j].GetImePrezime();
				}
				if (j < scorers2.size()) {
					if (j < scorers1.size()) {
						COUT << "              ";
					}
					else {
						COUT << "                          ";
					}
					COUT << scorers2[j].GetImePrezime();
				}
				COUT << endl;
			}
		}
		return COUT;
	}

	vector<Igrac*> operator()(int broj) {
		vector<Igrac*> novi{}; 
		
		for (int i = 0; i < _utakmice.getTrenutno(); i++) {
			auto& rep1 = _utakmice.getElement1(i);
			auto& rep2 = _utakmice.getElement2(i);

			for (size_t j = 0; j < rep1.GetIgraci().size(); j++)
			{
				auto igrac = rep1.GetIgraci()[j];
				auto pogodak = rep1.GetIgraci()[j].GetPogoci().size();

				if (pogodak >= broj) {
					novi.push_back(new Igrac(igrac));
				}
			}
			for (size_t j = 0; j < rep1.GetIgraci().size(); j++)
			{
				auto igrac = rep2.GetIgraci()[j];
				auto pogodak = rep2.GetIgraci()[j].GetPogoci().size();

				if (pogodak >= broj) {
					novi.push_back(new Igrac(igrac));
				}
			}
		}
		return novi;
	}
	

};

void main() {
	/*
	Globalna funkcija GenerisiID vraca ID igraca na osnovu vrijednosti
	proslijedjenih parametara. Funkcija generise
	ID sa maksimalno 4 cifre, ne racunajuci inicijale tj. prva slova
	imena i prezimena.
	Podrazumijeva se da ce uvijek biti proslijedjene validne
	vrijednosti parametara. Ukoliko se proslijede
	mala pocetna slova imena ili prezimena, potrebno ih je
	konvertovati u velika.
	U
	okviru generisanog ID-a, proslijedjena cjelobrojna vrijednost
	treba biti u obrnutom redoslijedu cifara.
	*/
	cout << GenerisiID("Denis Music", 3) << endl;//treba vratiti D000M3
	cout << GenerisiID("Jasmin Azemovic", 14) << endl;//treba vratiti J00A41
	cout << GenerisiID("Goran Skondric", 156) << endl;//treba vratiti G0S651
	cout << GenerisiID("emina junuz", 1798) << endl;//treba vratiti EJ8971
	//Za validaciju ID-a koristiti funkciju ValidirajID koja treba, koristeci regex, osigurati postivanje osnovnih pravila
	//vezanih za format koja su definisana u prethodnom dijelu zadatka.

	if (ValidirajID("D000M3"))
			cout << "ID VALIDAN" << endl;
	if (ValidirajID("J00A41"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("G00S651"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("Ej8971"))
		cout << "ID NIJE VALIDAN" << endl;

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju 
	cout << kolekcija1 << crt;
	/* metoda InsertAt treba da doda vrijednosti prvog i drugog
	parametra na lokaciju koja je definisana trecim parametrom. Povratna
	vrijednost metode
	je objekat (pozivaoc metode, u konkretnom slucaju objekat
	kolekcija1) u okviru koga su, na definisanu lokaciju, dodati
	zahtijevani parametri.
	Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i
	kolekcija2, bi trebali posjedovati sljedeci sadrzaj:
	10 10
	0  0
	1  1
	2  2
	* ....
	*/
	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(10, 10, 0);
	cout << kolekcija2 << crt;
	/*Metoda RemoveRange prihvata lokacija OD i DO, te u tom opsegu
	uklanja sve elemente iz kolekcije. U slucaju da zahtijevani opseg ne
	postoji u kolekciji
	metoda treba baciti izuzetak. Na kraju, metoda treba da vrati
	pokazivac na novi objekat tipa kolekcija koji sadrzi samo uklonjene
	elemente*/
	Kolekcija<int, int, 10> kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << kolekcija3 << endl;
	cout << kolekcija1 << crt;
	/*kolekcija3 bi trebala sadrzavati sljedece elemente:
	0 0
	1 1
	2 2
	dok bi kolekcija1 trebala sadrzavati sljedece elemente:
	10 10
	3 3
	4 4
	......
	*/
	kolekcija1 = kolekcija3;
	cout << kolekcija1;

	Vrijeme
		prviPogodak201633(20, 16, 33),
		drugiPogodak202319(20, 23, 19),
		treciPogodak205108(20, 51, 8),
		cetvrtiPogodak210654(21, 6, 54);

	Igrac
		denis("Denis Music"), jasmin("Jasmin Azemovic"),goran("Goran Skondric"), adil("Adil Joldic");

	   if (strcmp(denis.GetID(), "D000M1") == 0 && strcmp(jasmin.GetID(), "J000A2") == 0)
		cout << "\nID se uspjesno generise!" << endl;

	Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654,"podaci o cetvrtom pogotku"); 


	Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
	BIH.AddIgrac(denis);
	BIH.AddIgrac(jasmin);
	ENG.AddIgrac(goran);
	ENG.AddIgrac(adil);
	try
	{
		BIH.AddIgrac(denis);//onemoguciti dodavanje istih igraca - provjeravati ID
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}
	Prventstvo euro2024;
	euro2024.AddUtakmicu(BIH, ENG);
	try
	{
		euro2024.AddUtakmicu(BIH, ENG);//onemoguciti ponovne susrete drzava tokom istog prvenstva
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}
	//omoguciti dodavanje pogotka po ID-u ili imenu i prezimenu 
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "D000M1",
		prviPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	//onemoguciti dodavanje istih pogodaka 
	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak)) 
		cout << "Pogodak NIJE uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "J000A2",
		drugiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak)) 
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak)) 
		cout << "Pogodak uspjesno dodat" << endl;
	//nakon svakog  evidentiranog pogotka, svim igracima te utakmice (pod pretpostavkom da su validne email adrese sa ID - ovima igraca),
	 //u zasebnom thread-u, poslati email. u razmaku od 2 sekunde, sa sljedecim sadrzajem :
	
	//	To: D000M1@euro2024.com
	//	From: info@euro2024.com
	//	Subject: Informacija

	//	Postovani,

	//	U 20:35:16 sati igrac Jasmin Azemovic je zabiljezio svoj 1 pogodak na ovoj utakmici.
	//	 Trenutni rezultat je:

	//	 BOSNA_I_HERCEGOVINA 2 : 0 ENGLESKA

	//	 Puno srece u nastavku susreta.
	//	 Neka bolji tim pobijedi.
	// 

	// //ispisuje detaljnije informacije o susretu, kako je navedeno u narednom ispisu
		cout << euro2024;

	/*
	-------------------------------------------
	(3) BOSNA_I_HERCEGOVINA : ENGLESKA (1)
	-------------------------------------------
	Denis Music             Goran Skondric
	Jasmin Azemovic
	Jasmin Azemovic
	-------------------------------------------
	*/

	//vraca sve igrace koji su na takmicenju postigli broj pogodaka 
	//koji je veci ili jednak proslijedjenoj vrijednosti
		vector<Igrac*> igraci = euro2024(2);
		cout << crt << "Igraci sa 2 pogotka:\n";
		for (size_t i = 0; i < igraci.size(); i++)
			cout << igraci[i]->GetImePrezime() << endl;

	cin.get();
	system("pause>0");
}
