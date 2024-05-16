//
//  main.cpp
//  kutuphane333
//
//  Created by Mehmet Tuna Arıkaya on 17.05.2024.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Ogrenci {
  string adSoyad;
  string ogrenciNo;
  string bolum;
};

struct Kitap {
  string ISBN;
  string kitapAdi;
  string yazar;
  int sayfaSayisi;
};

struct Odunc {
  string ogrenciNo;
  string ISBN;
  string oduncAlmaTarihi;
  string geriVermeTarihi;
};

vector<Ogrenci> ogrenciOku(string dosyaAdi) {
  ifstream dosya(dosyaAdi);
  vector<Ogrenci> ogrenciListesi;
  if (dosya.is_open()) {
    string satir;
    while (getline(dosya, satir)) {
      Ogrenci ogrenci;
      size_t pos = satir.find(":");
      ogrenci.adSoyad = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      pos = satir.find(":");
      ogrenci.ogrenciNo = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      ogrenci.bolum = satir;
      ogrenciListesi.push_back(ogrenci);
    }
    dosya.close();
  }
  return ogrenciListesi;
}

vector<Kitap> kitapOku(string dosyaAdi) {
  ifstream dosya(dosyaAdi);
  vector<Kitap> kitapListesi;
  if (dosya.is_open()) {
    string satir;
    while (getline(dosya, satir)) {
      Kitap kitap;
      size_t pos = satir.find(":");
      kitap.ISBN = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      pos = satir.find(":");
      kitap.kitapAdi = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      pos = satir.find(":");
      kitap.yazar = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      kitap.sayfaSayisi = stoi(satir);
      kitapListesi.push_back(kitap);
    }
    dosya.close();
  }
  return kitapListesi;
}

vector<Odunc> oduncOku(string dosyaAdi) {
  ifstream dosya(dosyaAdi);
  vector<Odunc> oduncListesi;
  if (dosya.is_open()) {
    string satir;
    while (getline(dosya, satir)) {
      Odunc odunc;
      size_t pos = satir.find(":");
      odunc.ogrenciNo = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      pos = satir.find(":");
      odunc.ISBN = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      pos = satir.find(":");
      odunc.oduncAlmaTarihi = satir.substr(0, pos);
      satir.erase(0, pos + 1);
      odunc.geriVermeTarihi = satir;
      oduncListesi.push_back(odunc);
    }
    dosya.close();
  }
  return oduncListesi;
}

void oduncAl(vector<Odunc> &oduncListesi) {
  string ogrenciNo, ISBN;
  cout << "Ogrenci No: ";
  cin >> ogrenciNo;
  cout << "ISBN: ";
  cin >> ISBN;

  time_t t = time(0);
  struct tm *now = localtime(&t);
  char bugun[11];
  strftime(bugun, sizeof(bugun), "%d.%m.%Y", now);

  Odunc odunc;
  odunc.ogrenciNo = ogrenciNo;
  odunc.ISBN = ISBN;
  odunc.oduncAlmaTarihi = bugun;
  odunc.geriVermeTarihi = "00.00.0000";
  oduncListesi.push_back(odunc);

  ofstream dosya("Odunc_Tablosu.txt", ios::app);
  if (dosya.is_open()) {
    dosya << ogrenciNo << ":" << ISBN << ":" << bugun << ":00.00.0000" << endl;
    dosya.close();
  } else {
    cout << "Dosya acilamadi." << endl;
  }
}

void kitapIade(vector<Odunc> &oduncListesi) {
  string ogrenciNo, ISBN;
  cout << "Ogrenci No: ";
  cin >> ogrenciNo;
  cout << "ISBN: ";
  cin >> ISBN;

  time_t t = time(0);
  struct tm *now = localtime(&t);
  char bugun[11];
  strftime(bugun, sizeof(bugun), "%d.%m.%Y", now);

  for (auto &odunc : oduncListesi) {
    if (odunc.ogrenciNo == ogrenciNo && odunc.ISBN == ISBN) {
      odunc.geriVermeTarihi = bugun;
      break;
    }
  }

  ofstream dosya("Odunc_Tablosu.txt");
  if (dosya.is_open()) {
    for (const auto &odunc : oduncListesi) {
      dosya << odunc.ogrenciNo << ":" << odunc.ISBN << ":" << odunc.oduncAlmaTarihi << ":" << odunc.geriVermeTarihi << endl;
    }
    dosya.close();
  } else {
    cout << "Dosya acilamadi." << endl;
  }
}


int main() {
    vector<Ogrenci> ogrenciListesi = ogrenciOku("ogrenci_listesi.txt");
    vector<Kitap> kitapListesi = kitapOku("kitaplar.txt");
    vector<Odunc> oduncListesi = oduncOku("Odunc_Tablosu.txt");
    
    int secim;
    do {
        cout << "Yapmak istediginiz islemi secin:" << endl;
        cout << "1- Kitap odunc almak icin" << endl;
        cout << "2- Kitap iade etmek icin" << endl;
        cout << "3- Programdan cikmak icin" << endl;
        cout << "Secim: ";
        cin >> secim;
        
        switch (secim) {
            case 1:
                oduncAl(oduncListesi);
                break;
            case 2:
                kitapIade(oduncListesi);
                break;
            case 3:
                cout << "Programdan cikiliyor..." << endl;
                break;
            default:
                cout << "Gecersiz secim. Tekrar deneyin.";
                return 0 ;
        }
    } while (secim != 3);
    
    return 0;
}
