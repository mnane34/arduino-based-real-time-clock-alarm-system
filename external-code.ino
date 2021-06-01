#include<LiquidCrystal.h> // LCD Kütüphanesi Tanımlandı

LiquidCrystal lcd(13,12,11,10,9,8); // LCD Nesnesi Oluşturuldu

#include <DS3231.h>  // RTC Kütüphanesi Dahil Edildi

#define SDA A2  // RTC Pin
#define SCL A3

DS3231  rtc(SDA,SCL);  // RTC Nesnesi

Time  t; // Gerçek Zaman Değerine Ulaşmak İçin Gerekli bir nesne tanımlaması

int Saat; // RTC Değerleri
int Dakika;
int Gun;

int VeriSaat[2]; // Gelen Saat Bilgilerinin Tutulduğu Değişken

int VeriGun[7]; // Gelen Gün Bilgilerinin Tutulduğu Değişken

int SaatOnlar=0; // LCD Ekranında Düzgün Saat Bilgisi Basabilmek için gerekli değişkenler
int SaatBirler=0;
int DakikaOnlar=0;
int DakikaBirler=0;

struct Gun         // Gelen Gün Bilgileriyle İşlem Yapabilmek için Oluşturulmuş Değişkenler
{
  boolean Pazartesi=false;
  boolean Sali=false;
  boolean Carsamba=false;
  boolean Persembe=false;
  boolean Cuma=false;
  boolean Cumartesi=false;
  boolean Pazar=false;
  
}Veri;

void setup() 
{
  
  Serial.begin(9600); // Seri Port Baud Rate
  lcd.begin(16,2); // LCD Başlatıldı

  rtc.begin(); // RTC Başlatıldı

  // Yapılmak İstenen RTC Ayarlarının Kurulduğu Fonksiyonlar 

   //rtc.setDOW(7);               //  BU günü pazartesi olarak ayarladık
  //rtc.setTime(14, 1, 0);       //  saati 12:00:00 şeklinde ayarladık
  //rtc.setDate(21, 04, 2020);   // Tarihi 1/1/2014 olarak ayarladık

  Serial.print("*");
}

void loop()
{
  t = rtc.getTime(); // Saat Bilgisi Alabilmek için Nesne Tanımlaması gibi bişi
  
  Saat=(int)t.hour; // Saat Bilgisini Değişkene ata
  Dakika=(int)t.min; // Dakika Bilgisini Değişkene Ata
  
  SaatOnlar = Saat / 10; // LCD Ekrana Düzgün Basabilmek İçin Gerekli Matematiksel İşlemler
  SaatBirler = Saat % 10;
  DakikaOnlar = Dakika / 10;
  DakikaBirler = Dakika%10;
   
/* LCD Ekranda Sürekli Basılı Olmasını İstediklerim */
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(SaatOnlar);
  lcd.setCursor(1,0);
  lcd.print(SaatBirler);
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(3,0);
  lcd.print(DakikaOnlar);
  lcd.setCursor(4,0);
  lcd.print(DakikaBirler);
  lcd.setCursor(6,0);
  lcd.print(rtc.getDOWStr());
  lcd.setCursor(3,1);
  lcd.print(rtc.getDateStr());
  delay(200);

  if(Serial.available()>0)  // Haberleşme İşlemleri
  {
      for(int i=0; i<2; i++)
     {
    VeriSaat[i] = Serial.read();
    Serial.println(VeriSaat[i]);
    delay(200);
     }

     for(int j=0; j<7; j++)
     {
      VeriGun[j] = Serial.read();
      Serial.println(VeriGun[j]);
      delay(200);
     }
  }

/* Gelen Gün Bilgilerinin İşlenmesi */


  if(VeriGun[0]==1)
  {
    Veri.Pazartesi = true;
  }

  else if(VeriGun[1]==1)
  {
    Veri.Sali = true;
  }
  else if(VeriGun[2]==1)
  {
    Veri.Carsamba=true;
  }

  else if(VeriGun[3]==1)
  {
    Veri.Persembe=true;
  }

  else if(VeriGun[4]==1)
  {
    Veri.Cuma = true;
  }

  else if (VeriGun[5]==1)
  {
    Veri.Cumartesi=true;
  }

  else if(VeriGun[6]==1)
  {
    Veri.Pazar=true;
  }

    /* Slave Arduinoya Gelen Verilerin İşlenmesi */

     if (((int)t.hour == VeriSaat[0] )&& ((int)t.min == VeriSaat[1]) && Veri.Pazartesi == true)
  {
    Serial.write(1);
    delay(60000);
  }

    else  if (((int)t.hour == VeriSaat[0] )&& ((int)t.min == VeriSaat[1]) && Veri.Sali == true)
  {
    Serial.write(1);
    delay(60000);
  }

      else  if (((int)t.hour == VeriSaat[0] )&& ((int)t.min == VeriSaat[1]) && Veri.Carsamba == true)
  {
    Serial.write(1);
    delay(60000);
  }

      else  if (((int)t.hour == VeriSaat[0] )&& ((int)t.min == VeriSaat[1]) && Veri.Persembe == true)
  {
    Serial.write(1);
    delay(60000);
  }

      else  if (((int)t.hour == VeriSaat[0] )&& ((int)t.min == VeriSaat[1]) && Veri.Cuma == true)
  {
    Serial.write(1);
    delay(60000);
  }

}
