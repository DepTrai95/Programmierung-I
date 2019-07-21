#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>																	//Timer (Sleep) in Millisekunden
#define HASH_MAX 11																// 11 Speicherplätze (11 mögliche Prüfziffern)


struct buchregal {
	char isbn[11];														//wegen \0
	char autor[80];
	char titel[80];
	double preis;
	int anzahl;
	struct buchregal *next;
};
typedef struct buchregal buch;
buch *hash_tabelle[HASH_MAX];


double einundverkauf(double plusminus, double preis, int anzahl){
double static guthaben = 5000;
if (guthaben >= preis*anzahl){
		 if (plusminus == 0){															//einkaufen -> Wert verringern
			 guthaben = guthaben - (preis*anzahl);
			 return guthaben;
		 }
		 else if (plusminus == 1){															//verkaufen -> Wert erhoehen
			 guthaben = guthaben + (preis*anzahl);
			 return guthaben;
		 }
}
}

void kontostand_aktuell(){
	printf("Der Kontostand betraegt: %.2lf EUR\n", einundverkauf(0, 0, 0));
	getchar(); system("cls"); fflush(stdin);											//alles clearen (optik)
}

int isbn_pruefung(char *isbn) {
	int length = strlen(isbn);												//int Laenge = string Laenge, in dem Fall 10
	if (length != 10) {														// wenn Länge ungleich 10 fuehre aus:
		printf("Die ISBN hat nicht die vorausgesetzte Laenge.\n");
		return 0;
	}
	if (length == 10) {								  						// wenn Laenge 10 betraegt fuehre aus:
		int summe = 0;
		for (int i = 0; i < 9; i++) {
			char zahl = isbn[i];
			if (zahl < '0' || zahl > '9') {
				printf("Die ISBN ist nicht korrekt, eine Nicht-Ziffer ist enthalten\n");
				return 0;
			}
			summe = summe + ((i + 1)* (zahl - '0'));
		}

		int pruefziffer = (summe % 11);

		if (pruefziffer < 10 && isbn[9] - '0' == pruefziffer) {
			printf("Die ISBN ist korrekt.\n");
			return 1;
		}
		else if (pruefziffer == 10 && (isbn[9] == 'X' || isbn[9]=='x') ) {
			printf("Die ISBN ist korrekt.\n");
			return 1;

		}
		else printf("Die ISBN ist nicht korrekt wegen der Pruefziffer.\n");
		return 0;
	}
}

int pruefziffer_berechnung(char *isbn) {								//dient als Ersatz fuer Hash-Adresse
	int zahl_int, summe = 0;
	for (int i = 0; i < 9; i++) {
		zahl_int = isbn[i] - '0';										// Umrechnung isbn string zu int
		if (isbn[9] == 'x' || isbn[9] == 'X') zahl_int = 10;			// wenn 10. Ziffer x/X ist -> Wert 10
		summe = summe + ((1 + i)*zahl_int);								// bis 9. Stelle multiplizieren -> mod 11 = Pruefziffer
	}
	int pruefziffer = (summe % 11);
	return pruefziffer;
}

buch *einkaufen_neu(char *isbn, char *autor, char *titel, double preis, int anzahl) {
	buch *pointer;
	int hash_adresse = pruefziffer_berechnung(isbn);
	pointer = hash_tabelle[hash_adresse];
		while (pointer != NULL) {
			if (strcmp(isbn, pointer->isbn) == 0) {
				system("cls");  fflush(stdin);										//Bildschirm clearen (optik)
				printf("Das Buch mit der ISBN: %s ist bereits vorhanden\n\n\n", isbn);
				getchar(); system("cls"); fflush(stdin);
				return pointer;
			}
			pointer = pointer->next;
		}

		// Speicher für neues Element allozieren
		pointer = malloc(sizeof(buch));
		if (pointer == NULL) {
			printf("Kein Speicher für neue Buecher vorhanden\n");
			return NULL;
		}

		double guthaben = einundverkauf(0, 0, 0);
		double guthaben_einkaufen_neu = einundverkauf(0, preis, anzahl);
		if (guthaben >= guthaben_einkaufen_neu){								//aktuelles guthaben muss größer sein als preis*summe von eingegebenen Daten
			strcpy(pointer->isbn, isbn);
			strcpy(pointer->autor, autor);
			strcpy(pointer->titel, titel);
			pointer->preis = preis;
			pointer->anzahl = anzahl;
			pointer->next = hash_tabelle[hash_adresse];
			hash_tabelle[hash_adresse] = pointer;
			printf("\nNeues Buch erfolgreich hinzugefuegt.\n\n\n\n\n");
			return pointer;
		}
		return NULL;
}

int einkaufen(char *isbn) {
buch *pointer;
int hash_adresse = pruefziffer_berechnung(isbn);
pointer = hash_tabelle[hash_adresse];
double guthaben = einundverkauf(0, 0, 0);
double einkaufen;
if (pointer != NULL){
		if (strcmp(pointer->isbn, isbn) == 0) {
	  einkaufen = einundverkauf(0, pointer->preis, 1);
				if (guthaben > einkaufen){
						pointer->anzahl++;
						printf("Buch wurde erfolgreich hinzugefuegt.\n");
						Sleep(2000); system("cls"); fflush(stdin);							//Bildschirm clearen (optik)
						return 0;
						}
					}
					pointer = pointer->next;
}
printf("Es befindet sich kein Buch mit der ISBN: %s in der Datenbank.\n", isbn);
getchar();  system("cls");  fflush(stdin);													//Bildschirm clearen (optik)
return 1;
}

int verkaufen(char *isbn){
buch *pointer;
buch *temp;
int hash_adresse = pruefziffer_berechnung(isbn);
pointer = hash_tabelle[hash_adresse];
int i = 0;
temp=NULL;
while (pointer != NULL) {
	if (strcmp(pointer->isbn, isbn) == 0) {
		pointer->anzahl--;
		printf("Buch wurde erfolgreich verkauft.\n");
		Sleep(2000);  system("cls");  fflush(stdin);										//Bildschirm clearen (optik)
		if (pointer->anzahl == 0) {
			if (i == 0) {
				hash_tabelle[hash_adresse] = pointer->next;
				free(pointer);
			}
			if (i > 0) {
				temp->next = pointer->next;
				temp = hash_tabelle[hash_adresse];
				hash_tabelle[hash_adresse] = temp;
				free(pointer);
			}
			printf("Das Buch wurde aus dem System entfernt, da kein weiteres Exemplar vorhanden ist.\n");
			Sleep(2000);  system("cls");  fflush(stdin);								//Bildschirm clearen (optik)
		}
		return 1;
	}
	temp = pointer;
	pointer = pointer->next;
	i++;
}

	printf("Es befindet sich kein Buch mit der ISBN: %s in der Datenbank.\n", isbn);
	getchar();  system("cls");  fflush(stdin);												//Bildschirm clearen (optik)
	return 1;
}

void bestand(){
	buch *pointer;
	for (int i = 0; i < HASH_MAX; i++){
		pointer = hash_tabelle[i];
		printf("\nHashtabelle %i:", i);
			while (pointer != NULL){
				printf ("\nISBN: %s Autor: %s Titel: %s Preis: %.2lfEUR Anzahl: %i", pointer->isbn, pointer->autor, pointer->titel, pointer->preis, pointer->anzahl);
				pointer = pointer->next;
		}
	}
	getchar();  system("cls");  fflush(stdin);										//Bildschirm clearen (optik)
}

void wert_bestand(){
	buch *pointer;
	double bestandssumme = 0;
	for (int i = 0; i < HASH_MAX; i++){
		pointer = hash_tabelle[i];
		while (pointer != NULL){
			bestandssumme = bestandssumme + (pointer->preis*pointer->anzahl);
			pointer = pointer->next;
		}
	}
	printf ("Wert aller Buecher betraegt: %.2lf EUR\n", bestandssumme);
 	getchar(); system("cls"); fflush(stdin);										//Bildschirm clearen (optik)
}

void suche_a(char *autor) {
	buch *pointer;
	int gefunden = 0;
	strtok(autor, "\n");  // entfernt \n aus titel --> bei eingabe steht in titel "Mein\n", wenn er dann vergleicht sucht der nach 'Mein\n' statt nach 'Mein'
	printf("Folgende Buecher haben '%s' als Autor:\n", autor);
	for (int i = 1; i < HASH_MAX; i++) {
		pointer = hash_tabelle[i];
		if (pointer != NULL) {
			if (strstr(pointer->autor, autor)) {
				printf("ISBN: %s Autor: %s Titel: %s Preis: %.2lf Anzahl: %i\n", pointer->isbn, pointer->autor, pointer->titel, pointer->preis, pointer->anzahl);
				gefunden = 1;
				getchar(); system("cls"); fflush(stdin);							//Bildschirm clearen (optik)
			}
			pointer = pointer->next;
		}
	}

	if (gefunden == 0) {
		printf("Keine uebereinstimmenden Werke gefunden.\n");
		getchar(); system("cls"); fflush(stdin);						//Bildschirm clearen (optik)
	}
}

void suche_t(char *titel) {
	buch *pointer;
	int gefunden = 0;
	strtok(titel, "\n");  // entfernt \n aus titel --> bei eingabe steht in titel "Mein\n", wenn er dann vergleicht sucht der nach 'Mein\n' statt nach 'Mein'
	printf("Folgende Buecher enthalten '%s' im Titel:\n",titel);
	for (int i = 1; i < HASH_MAX; i++) {
		pointer = hash_tabelle[i];
		if (pointer != NULL) {
			if (strstr(pointer->titel, titel)) {
				printf("ISBN: %s Autor: %s Titel: %s Preis: %.2lf Anzahl: %i\n", pointer->isbn, pointer->autor, pointer->titel, pointer->preis, pointer->anzahl);
				gefunden = 1;
				getchar(); system("cls"); fflush(stdin);						//Bildschirm clearen (optik)
			}
			pointer = pointer->next;
		}
	}
	if (gefunden == 0) {
		printf("Keine uebereinstimmenden Werke gefunden.\n");
		getchar(); system("cls"); fflush(stdin);						//Bildschirm clearen (optik)
	}
}


int optionen(int wahl) {
	buch *pointer;
	char isbn[11], autor[80], titel[80];
	double preis;
	int anzahl;
	int i = 0;

	switch (wahl) {
	case 1: 																				//neuen Datensatz hinzufuegen
		do {
			printf("Geben sie die 10 - stellige ISBN Nummer ein : ");
			scanf("%s", &isbn);
		} while (isbn_pruefung(isbn) != 1);
		while (getchar() != '\n');
	//
		printf("Geben sie den Autor ein: ");
		fgets(autor, 80, stdin);
	//
		printf("Geben sie den Titel des Buches ein: ");
		fgets(titel, 80, stdin);
	//
		while (i != 1){
		printf("Geben sie den Preis fuer das Buch ein: ");
		scanf("%lf", &preis);
		while (getchar() != '\n');
		if (preis > 0){
			i = 1;
		}
	}
	//
		while (i != 2){
		printf("Geben sie die Anzahl der Buecher an:  ");
		scanf("%i", &anzahl);
		while (getchar() != '\n');
		if (anzahl > 0){
			i = 2;
		}
	}

 if((anzahl*preis) < einundverkauf(0,0,0)){
	 printf("Daten hinzugefuegt.\n\n");
	 printf("ISBN: %s\nAutor: %sTitel: %sPreis: %.2lf EUR\nAnzahl: %i Stueck\n", isbn, autor, titel, preis, anzahl);
	 einkaufen_neu(isbn, autor, titel, preis, anzahl);
 }
 else { printf("\nIhr Guthaben reicht leider nicht mehr aus.\n\n");
 getchar(); system("cls"); fflush(stdin);						//Bildschirm clearen (optik)
 }
		break;
	case 2: printf("Geben sie die ISBN des Buches an dass sie kaufen moechten: ");
		fgets(isbn, 11, stdin);
		einkaufen(isbn);															//Buecheranzahl erhoehen um 1
		break;
	case 3: printf("Geben sie die ISBN des Buches an dass sie verkaufen moechten: ");
		fgets(isbn, 11, stdin);
		verkaufen(isbn);															//Buecheranzahl um 1 verringern
		break;
	case 4: bestand();																//Bestandsabfrage in ganzzahligem Wert
		break;
	case 5: wert_bestand();															//Bestandsabfrage in Euro
		break;
	case 6: printf("Geben sie den Namen des gesuchten Autors an: ");
		fgets(autor, 80, stdin);
		suche_a(autor);																			//Autor Suche
		break;
	case 7: printf("Geben sie einen Begriff ein der im Titel vorhanden ist: ");
		fgets(titel, 80, stdin);
		suche_t(titel);																		//Titel Suche
		break;
	case 8: kontostand_aktuell();
		break;
	case 0: printf("Programm wird beendet.\n\n");
		exit (0);																					//Beende Programm
		break;
	default: printf("Ungueltige Eingabe. Wiederholen sie die Eingabe. Enter druecken.\n\n");
	 getchar(); system("cls");	fflush(stdin);
	}
}

int main(void) {
	einkaufen_neu("3836228610", "Thomas Theis", "Einstieg in Python: Ideal fuer Programmieranfaenger geeignet", 24.90, 2);
	einkaufen_neu("3527710981", "Stephen Randy Davis", "C++ fuer Dummies", 19.99, 4);
	einkaufen_neu("3868994122", "Daniel J. Barret", "Linux kurz & gut", 12.90, 4);
	einkaufen_neu("3898645363", "Jon Erickson", "Hacking: Die Kunst des Exploits (mit CD)", 46.00, 1);
	einkaufen_neu("3551354014", "Joanne K. Rowling", "Harry Potter und der Stein der Weisen", 15.99, 3);
	einkaufen_neu("3551354065", "Joanne K. Rowling", "Harry Potter und der Halbblutprinz", 23.99, 2);
	einkaufen_neu("3662472775", "Boris Hollas", "Grundkurs Theoretische Informatik: Mit Aufgaben und Anwendungen", 24.99, 5);
	einkaufen_neu("3836241307", "Hans-Peter Habelitz", "Programmieren lernen mit Java: Aktuell zu Java 8", 19.90, 6);
	einkaufen_neu("3527709959", "Barry A. Burd", "Mit Java programmieren lernen fuer Dummies", 19.99, 5);
	einkaufen_neu("395561770X", "Mark Lutz", "Python kurz & gut", 14.90, 3);
	einkaufen_neu("1111111111", "DJ Painal", "Mein DJ-Life", 34.99, 5);
	einkaufen_neu("2222222222", "Thomas-David Griedel", "Meine Leben als Medieninformatiker", 19.99, 3);
	einkaufen_neu("3333333333", "Tien Nguyen Van", "Mein Geld und Ich", 24.99, 2);
	einkaufen_neu("4444444444", "Hai Thanh Do", "Mein 'Korean Model', das BGB und ich", 19.99, 2);
	einkaufen_neu("5555555555", "Duc Anh Nguyen", "Die Geschichte zweier Kindheitsfreunde - mit etlichen Illustrationen", 29.99, 3);
	system("cls"); fflush(stdin);
	buch *anfang = NULL;
	buch *neu = NULL;

	int wahl;
	while (1) {
			printf("Optionen:\n");
			printf("0 - Beenden \n");
			printf("1 - einkaufen_neu - Daten hinzufuegen \n");
			printf("2 - einkaufen \n");
			printf("3 - verkaufen \n");
			printf("4 - Bestand \n");
			printf("5 - Wertbestand \n");
			printf("6 - Suche nach Autor \n");
			printf("7 - Suche nach Titel \n");
			printf("8 - aktuelle Kontostandabfrage \n");
			printf("\nNach jeder  Ausgabe 'Enter' druecken zum leeren des Bilschirms.\n");
			printf("\nBitte waehlen Sie aus: ");

			scanf(" %i", &wahl);
			while (getchar() != '\n');
			optionen(wahl);

		}
		return 0;
	}
