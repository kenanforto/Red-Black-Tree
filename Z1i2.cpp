#include <iostream>
#include <stdexcept>
using namespace std;

class RBTree {
    private:
    enum Color { BLACK, RED };
    struct Node {
        Node* parent;
        Node* leftChild;
        Node* rightChild;
        Color color;
        int value;
    };

    Node *root;
    Node *TNIL;

    const char* colorToString(Color color) {
    return color == BLACK ? "BLACK" : "RED";
    }

 public:
    RBTree() {
        TNIL = new Node;
        TNIL->color = BLACK;
        root = TNIL;
    }

    void INORDER(Node* node) {
        if (node != TNIL) {
            INORDER(node->leftChild);
            cout << node->value << "(" << colorToString(node->color) << ") ";
            INORDER(node->rightChild);
        }
    }

    void PRINT_INORDER() {
        INORDER(root);
    }

    // Nakon rotacije, y postaje roditelj x
    void leftRotate(Node* x) {
        Node* y = x->rightChild;
        x->rightChild = y->leftChild; // Bitno, jer se desno dijete x pomice na lijevo dijete y

        if (y->leftChild != TNIL) {
            y->leftChild->parent = x;
        }

        y->parent = x->parent; // Bitno, jer y uzima poziciju x nakon rotacije

        if (x->parent == TNIL) {
            root = y;
        } else if (x == x->parent->leftChild) { // Ako je x bio lijevo dijete svog roditelja, postavi ga na y
            x->parent->leftChild = y;
        } else {
            x->parent->rightChild = y;
        }

        y->leftChild = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        // Cuvanje pokazivaca na lijevo dijete cvora y
        Node* x = y->leftChild;

        y->leftChild = x->rightChild;

        // Ako cvor x ima desno dijete, postavljanje njegovog roditelja na cvor y
        if (x->rightChild != TNIL) {
            x->rightChild->parent = y;
        }

        x->parent = y->parent;

        // Ako cvor y bio korijen, postavljanje korijena na cvor x
        if (y->parent == TNIL) {
            root = x;
        } 
        // Ako je cvor y bio lijevo dete svog roditelja, postavljanje lijevog deteta roditelja na x
        else if (y == y->parent->leftChild) {
            y->parent->leftChild = x;
        } 
        // U suprotnom, postavljanje desnog djeteta roditelja na x
        else {
            y->parent->rightChild = x;
        }

        x->rightChild = y;
        y->parent = x;
    }

    void RBInsert(int value) {
        Node* y = TNIL;
        Node* x = root;
        Node* z = new Node;      
        z->value = value;        

        while (x != TNIL) {
            y = x;
            if (z->value < x->value) {
                x = x->leftChild;
            } else {
                x = x->rightChild;
            }
        }

        z->parent = y;           
        if (y == TNIL) {
            root = z;
        } else if (z->value < y->value) {
            y->leftChild = z;
        } else {
            y->rightChild = z;    
        }

        z->leftChild = TNIL;
        z->rightChild = TNIL;
        z->color = RED;

        RBInsertFixup(z);
    }


    void RBInsertFixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->leftChild) {
                Node* y = z->parent->parent->rightChild; // Ujak

                // CASE 1: Ujak je crven
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;              // Pomjeranje z na njegovog djeda
                } else {
                    // CASE 2: Ujak je crn, a z je desno dijete
                    if (z == z->parent->rightChild) {
                        z = z->parent;
                        leftRotate(z);
                    }

                    // CASE 3: Ujak z je crn, a z je lijevo dijete
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->leftChild;

                // CASE 1: Ujak z je crven
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;              // Pomjeranje z na njegovog djeda
                } else {
                    // CASE 2: Ujak z je crn, a z je lijevo dijete
                    if (z == z->parent->leftChild) {
                        z = z->parent;
                        rightRotate(z);
                    }

                    // CASE 3: Ujak z je crn, a z je desno dijete
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }


    void RBTransplant(Node* u, Node* v) {
        // Ako cvor koji se zamjenjuje (u) nema roditelja, zamjena korijena stabla (root)
        if (u->parent == TNIL) {
            root = v;
        } else if (u == u->parent->leftChild) {
            // Ako je cvor (u) lijevo dijete svog roditelja, postavljanje nove lijeve djece roditelja (v)
            u->parent->leftChild = v;
        } else {
            // Inače, cvor (u) je desno dijete svog roditelja, postavljanje nove desne djece roditelja (v)
            u->parent->rightChild = v;
        }

        // Ako zamjenski cvor (v) nije TNIL (prazan čvor), postavljanje njegovog roditelja na roditelja zamjenjenog cvora (u)
        v->parent = u->parent;
    }

    Node* treeMinimum(Node* c) {
        while (c->leftChild != TNIL) {
            c = c->leftChild;
        }
        return c;
    }

    Node* findNode(int kljuc) {
        Node* tempRoot = root;
        while (tempRoot != TNIL && kljuc != tempRoot->value) {
            if (kljuc < tempRoot->value) {
                tempRoot = tempRoot->leftChild;
            } else {
                tempRoot = tempRoot->rightChild;
            }
        }
        return tempRoot;
    }

    void RBDelete(int value) {
        Node* z = TNIL;
        z = findNode(value);
        if (z==TNIL) throw std::domain_error("Cvor ne postoji!");
        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;

        if (z->leftChild == TNIL) {
            x = z->rightChild;
            RBTransplant(z, z->rightChild);
        } else if (z->rightChild == TNIL) {
            x = z->leftChild;
            RBTransplant(z, z->leftChild);
        } else {
            y = treeMinimum(z->rightChild);
            yOriginalColor = y->color;
            x = y->rightChild;

            if (y->parent == z) {
                x->parent = y;
            } else {
                RBTransplant(y, y->rightChild);
                y->rightChild = z->rightChild;
                y->rightChild->parent = y;
            }

            RBTransplant(z, y);
            y->leftChild = z->leftChild;
            y->leftChild->parent = y;
            y->color = z->color;
        }
        if (yOriginalColor == BLACK) {
            RBDeleteFixup(x);
        }
        delete z;
    }

    void RBDeleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->leftChild) {
                Node* w = x->parent->rightChild;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->rightChild;
                }
                if (w->leftChild->color == BLACK && w->rightChild->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->rightChild->color == BLACK) {
                        w->leftChild->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->rightChild;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->rightChild->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->leftChild;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->leftChild;
                }
                if (w->rightChild->color == BLACK && w->leftChild->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->leftChild->color == BLACK) {
                        w->rightChild->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->leftChild;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->leftChild->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }
};

int main() {
    RBTree RedBlackTree;

    int opcija;
    do {
        cout << "\nIzaberite jednu od ponuđenih opcija:\n";
        cout << "1) Umetanje novog čvora\n";
        cout << "2) INORDER ispis čvorova na ekranu\n";
        cout << "3) Brisanje čvora\n";
        cout << "4) Izlaz\n";
        cout << "\nUnesite broj opcije: ";
        cin >> opcija;

        switch (opcija) {
            case 1: {
                int kljuc;
                cout << "Unesite ključ čvora za umetanje: ";
                cin >> kljuc;
                RedBlackTree.RBInsert(kljuc);
                cout << "\nČvor sa ključem " << kljuc << " je uspješno umetnut.\n";
                break;
            }
            case 2: {
                cout << "\n-----------------PRINT INORDER----------------"<<endl<<endl;
                RedBlackTree.PRINT_INORDER();
                cout << "\n\n----------------------------------------------\n";
                break;
            }
            case 3: {
                try {
                    int kljuc;
                    cout << "Unesite ključ čvora za brisanje: ";
                    cin >> kljuc;
                    RedBlackTree.RBDelete(kljuc);
                    cout << "\nČvor sa ključem " << kljuc << " je uspješno obrisan.\n";
                    break;
                }
                catch (const std::exception& e) {
                    std::cerr << "Uhvaćen izuzetak: "<< e.what() << std::endl;
                    break;
                }
            }
            case 4: {
                cout << "\nIzlaz iz programa.\n";
                break;
            }
            default:
            cout << "\nOpcija ne postoji. Molim vas unesite opciju 1, 2, 3 ili 4!\n";
            }
    } while (opcija != 4);
    return 0;
}