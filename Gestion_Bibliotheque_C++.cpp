#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;


class Livre
{
private:
    string titre;
    string auteur;
    string isbn;
    bool disponible;

public:
    Livre(string t, string a, string i)
    {
        titre = t;
        auteur = a;
        isbn = i;
        disponible = true;
    }

    string getTitre()
    {
        return titre;
    }

    string getAuteur()
    {
        return auteur;
    }

    string getISBN()
    {
        return isbn;
    }

    bool estDisponible()
    {
        return disponible;
    }

    void modifier(string t, string a, string i)
    {
        titre = t;
        auteur = a;
        isbn = i;
    }

    void emprunter()
    {
        disponible = false;
    }

    void retourner()
    {
        disponible = true;
    }

    void afficher()
    {
        cout << "Titre : " << titre << endl;
        cout << "Auteur : " << auteur << endl;
        cout << "ISBN : " << isbn << endl;

        if (disponible == true)
        {
            cout << "Statut : Disponible" << endl;
        }
        else
        {
            cout << "Statut : Emprunte" << endl;
        }

        cout << "------------------------" << endl;
    }
};



class Bibliotheque
{
private:
    vector<Livre> livres;

    int trouverIndex(string isbn)
    {
        int i;

        for (i = 0; i < (int)livres.size(); i++)
        {
            if (livres[i].getISBN() == isbn)
            {
                return i;
            }
        }

        return -1;
    }

public:
    void ajouterLivre(string titre, string auteur, string isbn)
    {
        if (titre == "" || auteur == "" || isbn == "")
        {
            cout << "Les champs sont obligatoires." << endl;
            return;
        }

        if (trouverIndex(isbn) != -1)
        {
            cout << "Cet ISBN existe deja." << endl;
            return;
        }

        Livre livre(titre, auteur, isbn);
        livres.push_back(livre);

        cout << "Livre ajoute avec succes." << endl;
    }

    void afficherTous()
    {
        int i;

        if (livres.empty())
        {
            cout << "Aucun livre dans la bibliotheque." << endl;
            return;
        }

        for (i = 0; i < (int)livres.size(); i++)
        {
            livres[i].afficher();
        }
    }

    void rechercher(string recherche)
    {
        int i;
        bool trouve = false;

        for (i = 0; i < (int)livres.size(); i++)
        {
            if (livres[i].getISBN() == recherche ||
                livres[i].getTitre().find(recherche) != string::npos)
            {
                livres[i].afficher();
                trouve = true;
            }
        }

        if (trouve == false)
        {
            cout << "Aucun livre trouve." << endl;
        }
    }

    void modifierLivre(string isbn)
    {
        int index;
        string titre;
        string auteur;
        string nouvelISBN;

        index = trouverIndex(isbn);

        if (index == -1)
        {
            cout << "Livre introuvable." << endl;
            return;
        }

        if (livres[index].estDisponible() == false)
        {
            cout << "Le livre est emprunte." << endl;
            return;
        }

        cout << "Nouveau titre : ";
        getline(cin, titre);

        cout << "Nouvel auteur : ";
        getline(cin, auteur);

        cout << "Nouvel ISBN : ";
        getline(cin, nouvelISBN);

        if (titre == "" || auteur == "" || nouvelISBN == "")
        {
            cout << "Les champs sont obligatoires." << endl;
            return;
        }

        int autre = trouverIndex(nouvelISBN);

        if (autre != -1 && autre != index)
        {
            cout << "Cet ISBN existe deja." << endl;
            return;
        }

        livres[index].modifier(titre, auteur, nouvelISBN);
        cout << "Livre modifie avec succes." << endl;
    }

    void supprimerLivre(string isbn)
    {
        int index;

        index = trouverIndex(isbn);

        if (index == -1)
        {
            cout << "Livre introuvable." << endl;
            return;
        }

        if (livres[index].estDisponible() == false)
        {
            cout << "Le livre est emprunte." << endl;
            return;
        }

        livres.erase(livres.begin() + index);
        cout << "Livre supprime avec succes." << endl;
    }

    void emprunterLivre(string isbn)
    {
        int index;

        index = trouverIndex(isbn);

        if (index == -1)
        {
            cout << "Livre introuvable." << endl;
        }
        else if (livres[index].estDisponible() == false)
        {
            cout << "Le livre est deja emprunte." << endl;
        }
        else
        {
            livres[index].emprunter();
            cout << "Emprunt enregistre." << endl;
        }
    }

    void retournerLivre(string isbn)
    {
        int index;

        index = trouverIndex(isbn);

        if (index == -1)
        {
            cout << "Livre introuvable." << endl;
        }
        else if (livres[index].estDisponible() == true)
        {
            cout << "Le livre est deja disponible." << endl;
        }
        else
        {
            livres[index].retourner();
            cout << "Retour enregistre." << endl;
        }
    }

    void afficherParStatut(bool statut)
    {
        int i;
        bool trouve = false;

        for (i = 0; i < (int)livres.size(); i++)
        {
            if (livres[i].estDisponible() == statut)
            {
                livres[i].afficher();
                trouve = true;
            }
        }

        if (trouve == false)
        {
            cout << "Aucun livre dans cette categorie." << endl;
        }
    }

    void sauvegarder()
    {
        int i;
        ofstream fichier("bibliotheque.txt");

        if (!fichier)
        {
            cout << "Erreur d'ouverture du fichier." << endl;
            return;
        }

        for (i = 0; i < (int)livres.size(); i++)
        {
            fichier << livres[i].getTitre() << "|";
            fichier << livres[i].getAuteur() << "|";
            fichier << livres[i].getISBN() << "|";

            if (livres[i].estDisponible() == true)
            {
                fichier << 1;
            }
            else
            {
                fichier << 0;
            }

            fichier << endl;
        }

        fichier.close();
        cout << "Sauvegarde terminee." << endl;
    }

    void charger()
    {
        string ligne;
        string titre;
        string auteur;
        string isbn;
        string statut;

        ifstream fichier("bibliotheque.txt");

        if (!fichier)
        {
            cout << "Aucune sauvegarde trouvee." << endl;
            return;
        }

        livres.clear();

        while (getline(fichier, ligne))
        {
            stringstream ss(ligne);

            if (!getline(ss, titre, '|') ||
                !getline(ss, auteur, '|') ||
                !getline(ss, isbn, '|') ||
                !getline(ss, statut))
            {
                continue;
            }

            if (titre == "" || auteur == "" || isbn == "")
            {
                continue;
            }

            if (trouverIndex(isbn) != -1)
            {
                continue;
            }

            Livre livre(titre, auteur, isbn);

            if (statut == "0")
            {
                livre.emprunter();
            }

            livres.push_back(livre);
        }

        fichier.close();
        cout << "Chargement termine." << endl;
    }
};



void afficherMenu()
{
    cout << endl;
    cout << "===== GESTION BIBLIOTHEQUE =====" << endl;
    cout << "1. Ajouter un livre" << endl;
    cout << "2. Afficher tous les livres" << endl;
    cout << "3. Rechercher un livre" << endl;
    cout << "4. Modifier un livre" << endl;
    cout << "5. Supprimer un livre" << endl;
    cout << "6. Emprunter un livre" << endl;
    cout << "7. Retourner un livre" << endl;
    cout << "8. Afficher les livres disponibles" << endl;
    cout << "9. Afficher les livres empruntes" << endl;
    cout << "10. Sauvegarder" << endl;
    cout << "11. Charger" << endl;
    cout << "0. Quitter" << endl;
    cout << "================================" << endl;
}




int main()
{
    Bibliotheque biblio;

    int choix;
    string isbn;
    string titre;
    string auteur;
    string ligne;

    biblio.charger();

    do
    {
        afficherMenu();

        cout << "Votre choix : ";
        getline(cin, ligne);

        choix = atoi(ligne.c_str());

        switch (choix)
        {
            case 1:
                cout << "Titre : ";
                getline(cin, titre);

                cout << "Auteur : ";
                getline(cin, auteur);

                cout << "ISBN : ";
                getline(cin, isbn);

                biblio.ajouterLivre(titre, auteur, isbn);
                break;

            case 2:
                biblio.afficherTous();
                break;

            case 3:
                cout << "Titre ou ISBN a rechercher : ";
                getline(cin, isbn);

                biblio.rechercher(isbn);
                break;

            case 4:
                cout << "ISBN du livre a modifier : ";
                getline(cin, isbn);

                biblio.modifierLivre(isbn);
                break;

            case 5:
                cout << "ISBN du livre a supprimer : ";
                getline(cin, isbn);

                biblio.supprimerLivre(isbn);
                break;

            case 6:
                cout << "ISBN du livre a emprunter : ";
                getline(cin, isbn);

                biblio.emprunterLivre(isbn);
                break;

            case 7:
                cout << "ISBN du livre a retourner : ";
                getline(cin, isbn);

                biblio.retournerLivre(isbn);
                break;

            case 8:
                biblio.afficherParStatut(true);
                break;

            case 9:
                biblio.afficherParStatut(false);
                break;

            case 10:
                biblio.sauvegarder();
                break;

            case 11:
                biblio.charger();
                break;

            case 0:
                biblio.sauvegarder();
                cout << "Fermeture du programme." << endl;
                break;

            default:
                cout << "Choix invalide." << endl;
        }

    } while (choix != 0);

    return 0;
}
