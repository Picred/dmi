#include <iostream>
#include <fstream>
using namespace std;

//funzione che sia in grado di calcolare la somma delle chiavi contenute all'interno di uno specifico livello dell'albero.

template <class T> class Nodo{
private:
    T* key;
    Nodo<T>* left, *right, *parent;

public:
    Nodo(T key){
        this->key = new T(key);
        left = right = parent = NULL;
    }

    void setKey(T key)              {this->key = new T(key);}
    void setLeft(Nodo<T>* left)     {this->left = left;}
    void setRight(Nodo<T>* right)   {this->right = right;}
    void setParent(Nodo<T>* parent) {this->parent = parent;}

    T getKey()          {return *key;}           //dubbio
    Nodo<T>* getLeft()  {return left;}
    Nodo<T>* getRight() {return right;}
    Nodo<T>* getParent(){return parent;}
};

template <class T> class BST{
private:
    int n;
    Nodo<T>* root;

    Nodo<T>* _rsearch(T key, Nodo<T>* r){
        if(!r) return NULL;
        if(r->getKey()==key) return r;
        if(key < r->getKey()) return _rsearch(key, r->getLeft());
        else return _rsearch(key, r->getRight());
    }

    Nodo<T>* _rmin(Nodo<T>* r){
        if(!r) return NULL;
        if(!r->getLeft()) return r;
        return _rmin(r->getLeft());
    }

    Nodo<T>* _successore(Nodo<T>* r){
        if(!r) return NULL;
        if(r->getRight())
            return _rmin(r->getRight());
        Nodo<T>* p = r->getParent();
        while(p && p->getKey() < r->getKey())
            p = p->getParent();
        return p;
    }

    void _preorder(ostream& os, Nodo<T>* r){
        if(!r) return;
        os << r->getKey() << " ";
        _preorder(os, r->getLeft());
        _preorder(os, r->getRight());
    }

    void _inorder(ostream& os, Nodo<T>* r){
        if(!r) return;
        _inorder(os, r->getLeft());
        os << r->getKey() << " ";
        _inorder(os, r->getRight());
    }

    void _postorder(ostream& os, Nodo<T>* r){
        if(!r) return;
        _postorder(os, r->getLeft());
        _postorder(os, r->getRight());
        os << r->getKey() << " ";
    }

    int _prof2(Nodo<T>* r){
		if(!r) return -1;
		Nodo<T>* tmp = r;
		int p = 0;
		while(tmp && tmp!=root){
			tmp = tmp->getParent();
			p++;
		}
		return p;
	}

	int _altezza(Nodo<T>* r){
		if(!r) return -1;
		Nodo<T>* tmp = _rmin(root);
		int p = 0;
		while(tmp){
			int t = _prof(tmp->getKey(),r);
			if(p<t) p = t;
			tmp = _successore(tmp);
		}
		return p;
	}

	int _prof(T key, Nodo<T>* r){
		if(!r) return -1;
		Nodo<T>* tmp = r;
		int p = 0;
		while(tmp && tmp->getKey()!=key && tmp->getKey()){
			if(key < tmp->getKey()) tmp = tmp->getLeft();
			else tmp = tmp->getRight();
			p++;
		}
		return p;
	}

public:
    BST(){
        n = 0;
        root = NULL;
    }

    BST<T>& ins(T key){
        Nodo<T>* tmp = root;
        Nodo<T>* parent = NULL;
        Nodo<T>* x = new Nodo<T>(key);
        n++;
        while(tmp){
            parent = tmp;
            if(key <= tmp->getKey()) tmp = tmp->getLeft();
            else tmp = tmp->getRight();
        }
        if(!parent){
            root = x;
            return *this;
        }
        if(key <= parent->getKey()) parent->setLeft(x);
        else parent->setRight(x);
        x->setParent(parent);
        return *this;
    }

    BST<T>& canc(T key){
       Nodo<T>* x = _rsearch(key, root);
       if(!x) return *this;
       if(x->getLeft() && x->getRight()){
            Nodo<T>* y = _successore(x);
            x->setKey(y->getKey());
            y->setKey(key);
            x = y;
       }
       Nodo<T>* z = x->getLeft();
       if(!z) z = x->getRight();
       Nodo<T>* parent = x->getParent();
       if(!parent) root = z;
       else{
           if(parent->getLeft()==x) parent->setLeft(z); 
           else parent->setRight(z);
       }
       if(z) z->setParent(parent);
       n--;
       return *this;
    }

    int search(T key){
         return (_rsearch(key, root)!=NULL);
    }

    int prof2(T key){
		Nodo<T>* x = _search(key,root);
		if(x) return _prof2(x);
		return -1;
	}

	int altezza(){
		return _altezza(root);
	}


    T* successore(T key){
        Nodo<T>* tmp = _search(key, root);
        while(tmp){
            Nodo<T>* y = _successore(tmp);
            if(y) return new T(tmp->getKey());
        }
        return NULL;
    }

    T* min(){
        return _rmin(root);
    }

    void preorder(ostream& os){
        return _preorder(os, root);
    }

    void inorder(ostream& os){
        return _inorder(os, root);
    }

    void postorder(ostream& os){
        return _postorder(os, root);
    }

    void level_sum(int *v, int n,ostream& os){
		Nodo<T>* tmp = root;
		Nodo<T>* r = _rmin(tmp);
		while(r){
			int t = _prof2(r);
			v[t] += r->getKey(); 
			r = _successore(r);
		}
		for(int i=0; i<n; i++) os << v[i] << " ";
	}

};

int main(){
    ifstream in("input.txt");
    ofstream out("output.txt");

    string type;
    while(in >> type){
        int dim;
        in >> dim;

        if(type=="int"){
            BST<int> *b = new BST<int>();
            char c;
            int num;
            for(int i=0; i<dim; i++){
                in >> c;
                if(c == 'i'){
                    for(int count=0; count<=2; count++) in >> c;
                    in >> num;
                    b->ins(num);
                }
                else if(c == 'c'){
                    for(int count=0; count<=3; count++) in >> c;
                    in >> num;
                    b->canc(num);
                }
            }
            int n = b->altezza()+1;
            int v[n] = {};
           	b->level_sum(v,n,out);
        }

        if(type=="double"){
            BST<double> *b = new BST<double>();
            char c;
            double num;
            for(int i=0; i<dim; i++){
                in >> c;
                if(c == 'i'){
                    for(int count=0; count<=2; count++) in >> c;
                    in >> num;
                    b->ins(num);
                }
                else if(c == 'c'){
                    for(int count=0; count<=3; count++) in >> c;
                    in >> num;
                    b->canc(num);
                }
            }
            int n = b->altezza()+1;
            int v[n] = {};
            b->level_sum(v,n,out);
        }
        out << endl;
    }
    in.close();
    out.close();

    return 0;
}
