#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct date {
    int jour;
    int mois;
    int anne;
} date;

typedef struct client {
    int code_client;
    char nom[20];
    char prenom[20];
    int code_postale;
    date dn; // date de naissance
} client;

typedef struct compte {
    int code_client;
    int code_cpt; // code pin(4 chiffres)
    int rib; // 34 chiffres
    float sold; // sold > 250dt
    date nv; // date de creation de compte
} compte;

typedef struct bdclient {
    client info;
    struct bdclient *suiv;
} bdclient;

typedef struct bdcompte {
    compte info;
    struct bdcompte *suiv;
} bdcompte;

void init(bdclient **tete) {
    *tete = NULL;
}

int verif(bdclient *tete, int x) {
    bdclient *p = tete;
    while (p != NULL && p->info.code_client != x)
        p = p->suiv;
    if (p == NULL)
        return 1;
    else
        return 0;
}

void chercher(bdclient *tete, int id) {
    bdclient *p = tete;
    while (p != NULL && p->info.code_client != id)
        p = p->suiv;
    if (p == NULL)
        printf("Client non trouvé!\n");
    else {
        printf("Client trouvé:\n");
        printf("Nom: %s\n", p->info.nom);
        printf("Prenom: %s\n", p->info.prenom);
        printf("Code postal: %d\n", p->info.code_postale);
        printf("Date de naissance: %d/%d/%d\n",
               p->info.dn.jour, p->info.dn.mois, p->info.dn.anne);
    }
}

void ajout_client(bdclient **tete) {
    bdclient *p;
    int x;
    p = malloc(sizeof(bdclient));
    if (p == NULL) {
        printf("Erreur d'allocation de mémoire!\n");
        return;
    }

    do {
        printf("donner id de client :\n");
        scanf("%d", &x);
        p->info.code_client = x;
    } while (verif(*tete, x) == 0);

    printf("donner le nom de client :\n");
    scanf("%s", p->info.nom);
    printf("donner son prenom :\n");
    scanf("%s", p->info.prenom);
    printf("donner son code postale :\n");
    scanf("%d", &p->info.code_postale);
    printf("donner date de naissance (jour/mois/anne)\n");
    scanf("%d/%d/%d", &p->info.dn.jour, &p->info.dn.mois, &p->info.dn.anne);

    p->suiv = *tete;
    *tete = p;
    printf("done!\n");
}

void supprimer_client(bdclient **tete, int id) {
    bdclient *p = *tete;
    bdclient *prev = NULL;

    while (p != NULL && p->info.code_client != id) {
        prev = p;
        p = p->suiv;
    }

    if (p == NULL) {
        printf("Client non trouvé!\n");
        return;
    }

    if (prev == NULL)
        *tete = p->suiv;
    else
        prev->suiv = p->suiv;

    free(p);
    printf("Client supprimé avec succès!\n");
}

void chercher_acc(bdcompte *tete, int rib) {
    bdcompte *p = tete;
    while (p != NULL && p->info.rib != rib)
        p = p->suiv;

    if (p == NULL)
        printf("Compte non trouvé!\n");
    else {
        printf("Compte trouvé:\n");
        printf("Code client: %d\n", p->info.code_client);
        printf("RIB: %d\n", p->info.rib);
        printf("Solde: %.2f\n", p->info.sold);
        printf("Date de création: %d/%d/%d\n",
               p->info.nv.jour, p->info.nv.mois, p->info.nv.anne);
    }
}

void ajout_compte(bdcompte **tete_compte, bdclient *tete_client) {
    bdcompte *p;
    int client_id;

    printf("donner id du client:\n");
    scanf("%d", &client_id);

    if (verif(tete_client, client_id) == 1) {
        printf("Client non trouvé!\n");
        return;
    }

    p = malloc(sizeof(bdcompte));
    if (p == NULL) {
        printf("Erreur d'allocation de mémoire!\n");
        return;
    }

    p->info.code_client = client_id;
    printf("donner code pin (4 chiffres):\n");
    scanf("%d", &p->info.code_cpt);
    printf("donner RIB (34 chiffres):\n");
    scanf("%d", &p->info.rib);

    do {
        printf("donner solde initial (>250dt):\n");
        scanf("%f", &p->info.sold);
    } while (p->info.sold <= 250);

    printf("donner date de création (jour/mois/anne):\n");
    scanf("%d/%d/%d", &p->info.nv.jour, &p->info.nv.mois, &p->info.nv.anne);

    p->suiv = *tete_compte;
    *tete_compte = p;
    printf("Compte créé avec succès!\n");
}

void afficher(bdcompte *tete) {
    bdcompte *p = tete;
    if (p == NULL) {
        printf("Aucun compte trouvé!\n");
        return;
    }

    printf("\nListe des comptes:\n");
    printf("------------------\n");
    while (p != NULL) {
        printf("Code client: %d\n", p->info.code_client);
        printf("RIB: %d\n", p->info.rib);
        printf("Solde: %.2f\n", p->info.sold);
        printf("Date de création: %d/%d/%d\n\n",
               p->info.nv.jour, p->info.nv.mois, p->info.nv.anne);
        p = p->suiv;
    }
}

void s3menu(bdcompte **tete_compte) {
    int x, rib, code_pin;
    float montant;
    bdcompte *p;

    printf("-------------WELCOME------------\n");
    printf("1-Consulter solde\n");
    printf("2-Retrait\n");
    printf("3-Dépôt\n");
    printf("4-Retour\n");
    scanf("%d", &x);

    switch (x) {
        case 1:
        case 2:
        case 3:
            printf("Donner RIB:\n");
            scanf("%d", &rib);
            printf("Donner code PIN:\n");
            scanf("%d", &code_pin);

            p = *tete_compte;
            while (p != NULL && (p->info.rib != rib || p->info.code_cpt != code_pin))
                p = p->suiv;

            if (p == NULL) {
                printf("Compte non trouvé ou code PIN incorrect!\n");
                return;
            }

            if (x == 1) {
                printf("Solde: %.2f\n", p->info.sold);
            } else if (x == 2) {
                printf("Montant à retirer:\n");
                scanf("%f", &montant);
                if (montant > p->info.sold - 250) {
                    printf("Solde insuffisant!\n");
                    return;
                }
                p->info.sold -= montant;
                printf("Retrait effectué. Nouveau solde: %.2f\n", p->info.sold);
            } else {
                printf("Montant à déposer:\n");
                scanf("%f", &montant);
                p->info.sold += montant;
                printf("Dépôt effectué. Nouveau solde: %.2f\n", p->info.sold);
            }
            break;
        case 4:
            return;
        default:
            printf("Option invalide!\n");
    }
}

void s1menu(bdclient **tete) {
    int x;
    printf("-------------WELCOME------------\n");
    printf("1-rechercher un client (donner id) \n");
    printf("2-sign in new client \n");
    printf("3-supprimer une client \n");
    printf("4-retour\n");
    scanf("%d", &x);
    switch (x) {
        case 1:
            printf("donner id client à chercher :\n");
            scanf("%d", &x);
            chercher(*tete, x);
            break;
        case 2:
            ajout_client(tete);
            break;
        case 3:
            printf("donner id de client a supprimer :\n");
            scanf("%d", &x);
            supprimer_client(tete, x);
            break;
        case 4:
            return;
        default:
            printf("out of range !\n");
    }
}

void s2menu(bdcompte **tete_compte, bdclient **tete_client) {
    int x;
    printf("-------------WELCOME------------\n");
    printf("1-rechercher un compte (donner rib) \n");
    printf("2-ajouter un compte \n");
    printf("3-afficher la liste des compte \n");
    printf("4-fermer un compte \n");
    printf("5-retour\n");
    scanf("%d", &x);
    switch (x) {
        case 1:
            printf("donner rib de compte a chercher :\n");
            scanf("%d", &x);
            chercher_acc(*tete_compte, x);
            break;
        case 2:
            ajout_compte(tete_compte, *tete_client);
            break;
        case 3:
            afficher(*tete_compte);
            break;
        case 4:
            printf("Fonctionnalité à implementer\n");
            break;
        case 5:
            return;
        default:
            printf("out of range !\n");
    }
}

void menu(bdclient **tete, bdcompte **tete_compte) {
    int x;
    do {
        printf("-------------WELCOME------------\n");
        printf("1-gestion des client \n");
        printf("2-gestion des compte \n");
        printf("3-gestion des operation \n");
        printf("4-quitter \n");
        scanf("%d", &x);
        switch (x) {
            case 1:
                s1menu(tete);
                break;
            case 2:
                s2menu(tete_compte, tete);
                break;
            case 3:
                s3menu(tete_compte);
                break;
            case 4:
                printf("Au revoir!\n");
                exit(0);
            default:
                printf("out of range !\n");
        }
    } while (1);
}

int main() {
    bdclient *tete_client = NULL;
    bdcompte *tete_compte = NULL;
    menu(&tete_client, &tete_compte);
    return 0;
}
