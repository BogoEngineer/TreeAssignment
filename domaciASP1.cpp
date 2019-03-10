#include <iostream>
#include <math.h>
using namespace std;


class Node {
	int info;
	Node** pok;
	Node* otac;
	int kap, duz = 0;


	void kopiraj(const Node& N) {
		info = N.info;
		kap = N.kap;
		duz = N.duz;
		otac = N.otac;
		pok = new Node*[kap];
		for (int i = 0; i < duz; pok[i] = N.pok[i++]);
	}


	void premesti(Node& n) {
		info = n.info;
		kap = n.kap;
		duz = n.duz;
		otac = n.otac;
		pok = n.pok;
		pok = nullptr;
		otac = nullptr;
	}

	void brisi() {
		otac = nullptr;
		info = kap = duz = 0;
		delete[] pok;
		pok = nullptr;
	}

public:
	Node(int k, int i, Node* o = nullptr) {  //Podrazumevajuci konstruktor
		info = i;
		kap = k;
		otac = o;
		pok = new Node*[k];
		for (int i = 0; i < k; pok[i++] = nullptr);
	}

	Node(const Node& n) {                                                                   //Kopirajuci
		kopiraj(n);
	}

	Node(Node&& n) {                                                                        //Premestajuci
		premesti(n);
	}

	~Node() {                                                                               //Destruktor
		brisi();
	}

	Node& operator= (const Node& n) {                                                   //Kopirajuca dodela vrednosti
		if (this != &n) { brisi(); kopiraj(n); }
		return *this;
	}

	Node& operator= (Node&& n) {                                                    //Premestajuca
		if (this != &n) { brisi(); premesti(n); }
		return *this;
	}

	Node** getPok() const { return pok; }

	void incDuz() { duz++; };

	void setOtac(Node* o) { otac = o; }

	int getKap() const { return kap; }

	int& getDuz() { return duz; }

	int getInfo() const { return info; }

	Node* getOtac() const { return otac; }

	Node*& operator[] (int i) {
		return pok[i];
	}
	Node* goToBro() {
		int i = 0;
		if (otac == nullptr) return nullptr;
		while (this != (*otac)[i]) i++;
		if ((i + 1) == kap) return nullptr;
		return (*otac)[i + 1];
	}

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Tree {
	Node* root;
	int m;

	void brisi();

public:
	Tree(int k, int info) {
		m = k;
		root = new Node(m, info);

	}

	Tree(const Tree&) = delete;
	Tree& operator=(const Tree&) = delete;

	~Tree() {
		brisi();
	}
	void dodaj(int info);
	int brcvorova() const;
	int getHeight() const {
		Node* pom = (*root)[0];
		int visina = 0;
		while (pom != nullptr) {
			pom = (*pom)[0];
			visina++;
		}
		return visina;
	}
	friend ostream& operator<<(ostream& it, const Tree& t);

};



void Tree::dodaj(int info) {
	Node *n = new Node(m, info);
	Node  *brat, *pozicija = root, *pom = pozicija, *otac, *q = root;
	int h = getHeight();
	int maxBrCvorova = (pow(m, h + 1) - 1) / (m - 1);
	if (brcvorova() == maxBrCvorova) {
		while ((*q)[0] != nullptr) {
			q = (*q)[0];
		}
		(*q)[0] = n;
		n->setOtac(q);
		q->incDuz();
	}
	else {
		while (pozicija->getDuz() == pozicija->getKap()) { //nalazi poziciju oca kom treba da se umetne novi cvor
			brat = pozicija->goToBro();
			if (brat != nullptr) { pozicija = brat; }
			else {
				pom = (*pom)[0];
				pozicija = pom;
			}
		}
		otac = pozicija;
		(*otac)[otac->getDuz()] = n;
		n->setOtac(otac);
		otac->incDuz();
	}
}

ostream& operator<<(ostream& it, const Tree& t) {
	Node* pom, *q, *brat;
	int dim = t.getHeight() + 1;
	Node** stack = new Node*[dim];
	for (int i = 0; i < dim; stack[i++] = nullptr);
	int i = 0;
	stack[i] = t.root;
	while (stack[0] != nullptr) {
		pom = stack[i];
		stack[i--] = nullptr;
		while (pom != nullptr) {
			q = (*pom)[0];
			if (pom->getOtac() == nullptr) brat = nullptr;
			else {
				brat = pom->goToBro();
			}
			if (brat != nullptr) {
				stack[++i] = brat;
			}
			it << pom->getInfo();
			it << " ";
			pom = q;
		}
	}
	it << endl;
	delete[] stack;
	return it;
}


void Tree::brisi() {
	Node* pom = root;
	Node** niz = new Node*[brcvorova()];
	bool flag = false;
	for (int i = 0; i < brcvorova(); niz[i++] = nullptr);
	int i = 0;
	int counter;
	niz[i] = root;
	pom = niz[i--];
	while (pom != nullptr) {  //puni stek svim cvorovima pocevsi od korena iduci od najdesnijeg sina ka najlevljem
		if ((*pom)[0] == nullptr) {
			flag = true;
			counter = 0;
			if (pom->getOtac() != nullptr) {
				while (pom != (*(pom->getOtac()))[counter]) counter++; //da moze da namesti pokazivac na cvor koji ce biti obrisan na nullptr
				(*pom)[counter] = nullptr;
			}
			delete pom;
		}
		if (!flag) {
			if ((*(*pom)[0])[0] == nullptr) {                         //nema unuce
				if (pom->getOtac() != nullptr) {
					counter = 0;
					while (pom != (*(pom->getOtac()))[counter]) counter++;
					(*(pom->getOtac()))[counter] = nullptr;
				}
				for (int j = 0; j < pom->getDuz(); j++) { delete ((*pom)[j]); (*pom)[j] = nullptr; }
				delete pom;
			}
			else {
				for (int j = pom->getDuz() - 1; j >= 0; j--) niz[++i] = (*pom)[j];
			}
		}
		if (i < 0) break;
		pom = niz[i--];
		flag = false;
	}
	root = nullptr;
	delete[] niz;
	niz = nullptr;
}

//void Tree::brisi() {
//	Node* pom = root;
//	Node* q;
//	bool flag;
//	int i;
//	while (pom != nullptr) {
//		flag = false; //jos nisu izbrisana deca pa ne treba ici na oca/brata nego na decu
//		i = 0;
//		if (pom->imaDecu()) {
//			pom = pom->goToLeaf();
//			flag = true;
//		}
//		else {
//			q = pom->getOtac();
//			while ((*q)[i] != pom) { i++; }
//			delete pom;
//			pom = nullptr;
//			(*q)[i] = nullptr;
//		}
//		if (!flag) {
//			if (pom->goToBro() != nullptr)pom->goToBro();
//			else pom = pom->getOtac();
//		}
//	}
//}

int Tree::brcvorova() const {
	int counter = 0;
	Node *pom, *q, *brat;
	int dim = getHeight() + 1;
	Node** stack = new Node*[dim];
	for (int i = 0; i < dim; stack[i++] = nullptr);
	int i = 0;
	stack[i] = root;
	while (stack[0] != nullptr) {
		pom = stack[i];
		stack[i--] = nullptr;
		while (pom != nullptr) {
			q = (*pom)[0];
			counter++;
			if (pom->getOtac() == nullptr) brat = nullptr; //ako nema oca onda nema ni brata jer je koren
			else {
				brat = pom->goToBro();
			}
			if (brat != nullptr) {
				stack[++i] = brat;
			}
			pom = q;
		}
	}
	return counter;
}

int main() {
	int izbor;
	Tree*  T = nullptr;
	bool radi = false;
	while (!radi) {
		cout << "Izaberite zeljenu radnju:" << endl;
		cout << "1) Kreiraj stablo" << endl;
		cout << "2) Dodaj element" << endl;
		cout << "3) Prebroj cvorove" << endl;
		cout << "4) Ispisi stablo" << endl;
		cout << "5) Brisi stablo" << endl;
		cin >> izbor;
		switch (izbor) {
		case 1: {
			if (T != nullptr) {
				cout << "Stablo je vec napravljeno";
				break;
			}
			int info, m;
			cout << "Unesite podatak korena i stepen stabla: " << endl;
			cin >> info >> m;
			T = new Tree(m, info);
			break;
		}
		case 2: {
			if (T == nullptr) {
				cout << "Ne postoji stablo!" << endl;
				break;
			}
			int info;
			cout << "Vrednost novog cvora? : " << endl;
			cin >> info;
			T->dodaj(info);
			break;
		}
		case 3: {
			if (T == nullptr) {
				cout << "Ne postoji stablo!" << endl;
				break;
			}
			cout << T->brcvorova();
			break;
		}
		case 4: {

			if (T == nullptr) {
				cout << "Ne postoji stablo!" << endl;
				break;
			}
			cout << *T;
			break;
		}
		case 5: {
			delete T;
			T = nullptr;
			break;
		}
		default: {
			cout << "Pokusaj ponovo." << endl;
			break;
		}
		}
	}
	delete T;
	T = nullptr;
	return 0;


}