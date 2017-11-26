
void ReadFile(char* nazwa)
{
 lns(2);
 FILE* plik;
 if ((plik = fopen(nazwa,"r+")) == NULL)
 	{
    cout<<"Brak pliku: "<<nazwa<<"\n";
    lns(2);
    return;
   }
 int zn;
 while ((zn = fgetc(plik)) != EOF)
 	{
    cout<<(char)zn;
   }
 fclose(plik);
 lns(2);
}

void Ask(TELE* spis)
{
 cout<<"Konczysz dzialanie programu, zapisac? ";
 char zn;
 takechar(&zn);
 if (zn == 't') spis->ZapiszTelefony();
}

void ExecuteCmd(char* cmd, TELE* spis)
{
 if (strcmp(cmd,"h") == 0) ReadFile("Dialog/Help");
 else if (strcmp(cmd,"d") == 0) spis->DodajTelefon();
 else if (strcmp(cmd,"z") == 0) spis->ZobaczTelefony();
 else if (strcmp(cmd,"s") == 0) spis->ZapiszTelefony();
 else if (strcmp(cmd,"k") == 0) Ask(spis);
 else if (strcmp(cmd,"l") == 0) spis->WczytajTelefony();
 else if (strcmp(cmd,"u") == 0) spis->UsunTelefon();
 else if (strcmp(cmd,"m") == 0) spis->ModyfikujTelefon();
 else if (strcmp(cmd,"a") == 0) spis->ZobaczWszystko();
 else if (strcmp(cmd,"w") == 0) spis->Szukaj();
 else if (strcmp(cmd,"g") == 0) spis->GenerujTXT();
 else if (strcmp(cmd,"c") == 0) spis->CheckOut();
 else if (strcmp(cmd,"t") == 0) spis->Sortuj();
 else cout<<"Nieznane polecenie, wpisz h aby zobaczyc liste polecen.\n";
}

void Wykonaj(TELE* spis)
{
 char cmd[STRING];
 strcpy(cmd,"(null)");
 ReadFile("Dialog/Info");
 while (strcmp(cmd,"k") != 0)
 	{
    cout<<"MorgothDBMA Linux "<<spis->PokazNazwe()<<" -> ";
    takestring(cmd);
    downcase(cmd);
    ExecuteCmd(cmd,spis);
   }
}

