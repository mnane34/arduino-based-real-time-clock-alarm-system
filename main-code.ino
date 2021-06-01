#include <LiquidCrystal.h> // LCD Kütüphanesi Tanımlandı

LiquidCrystal lcd(8,9,4,5,6,7);  // Bu Pinleri Değiştiremeyiz. Uno üzerine Direkt Oturuyor.


struct LcdKeypad                // LCD KEYPAD 
{
const int ArkaPlanisik=10 ;

int BasilanTus=0; 
int OkunanDeger=0;

const int Sag=0 ;
const int Yukari=1;
const int Asagi=2;
const int Sol=3;
const int Enter=4;
const int Yok=5;

}LCDKEYPAD;


                        /* Kullanılan Özel Karakterler */

byte OzelKarakter1[]={0x0,0x1,0x3,0x16,0x1C,0x8,0x0,0x0}; // Tik İşareti Karakteri

byte OzelKarakter2[]={0x10,0x18,0x1C,0x1E,0x1E,0x1C,0x18,0x10}; // İmlec Karakteri

byte OzelKarakter0[]={0x0,0x11,0xA,0x4,0x4,0xA,0x11,0x0}; // Çarpı İşareti Karakteri

byte OzelKarakter3[]={0x4,0xE,0x15,0x4,0x4,0x4,0x4,0x4}; // Yukarı İmlec Karakteri


int ButonDurum=0; // Ekranlar Arası Geçiş için Kullanılacak Olan Değişken

struct Mod1 // Açılış Penceresi 1.Pencere
{
  int imlecDurum=1;
}EKRAN1;

struct Mod2 // Açılış Penceresi 2.Pencere
{
  byte SaatOnlar=0;
  byte SaatBirler=0;
  byte DakikaOnlar=0;
  byte DakikaBirler=0;

  int imlecDurum=1;

  int imlecKaydirma=4;
  
}EKRAN2;

struct Mod3
{

  byte CursorPazartesi=3;
  byte CursorSali=3;
  byte CursorCarsamba=3;
  byte CursorPersembe=3;
  byte CursorCuma=3;
  byte CursorCumartesi=3;
  byte CursorPazar=3;

}EKRAN3;


byte ButonOku()                  // LCD Keypad Buton Okuma Fonksiyonu
{
  LCDKEYPAD.OkunanDeger=analogRead(A0);

  if(LCDKEYPAD.OkunanDeger > 1000)
  {
    return LCDKEYPAD.Yok;
  }

  else if (LCDKEYPAD.OkunanDeger <50)
  {
    return LCDKEYPAD.Sag;
  }

    else if (LCDKEYPAD.OkunanDeger <195)
  {
    return LCDKEYPAD.Yukari;
  }

      else if (LCDKEYPAD.OkunanDeger <380)
  {
    return LCDKEYPAD.Asagi;
  }

      else if (LCDKEYPAD.OkunanDeger <555)
  {
    return LCDKEYPAD.Sol;
  }

        else if (LCDKEYPAD.OkunanDeger <790)
  {
    return LCDKEYPAD.Enter;
  }

  //   0   LCDKEYPAD.BasilanTus = "Sag"
  //   1   LCDKEYPAD.BasilanTus = "Yukari"
  //   2   LCDKEYPAD.BasilanTus = "Asagi"
  //   3   LCDKEYPAD.BasilanTus = "Sol"
  //   4   LCDKEYPAD.BasilanTus = "Enter"
  //   5   LCDKEYPAD.BasilanTus = "Basilmiyor"  
 
}

int Saat=0;   /*Ayarlanan Saat Değerleri*/
int Dakika=0;

int GunDizisi[7]; /*Ayarlanan Gün Değerleri*/

int GelenData = 0; // Slave'den Alarm Bilgisi Tutulduğu değişken

#define BUZZER 28    /* Pin Atamaları */

#define LatchPin 24
#define ClockPin 26
#define DataPin 22

byte Leds = 0; // Karaşimşek Led Durumu

int GecikmeSuresi = 30; // Karaşimşek Gecikme Süresi Değişkeni

unsigned long eskiZaman=0; // Millis Fonksiyonları için kullanılan değişkenler
unsigned long yeniZaman;


void setup()
{
   
  lcd.begin(16,2); // LCD Başlatıldı
  
  pinMode(LCDKEYPAD.ArkaPlanisik,OUTPUT); // Çıkış İşlemi Yapıldı
  digitalWrite(LCDKEYPAD.ArkaPlanisik,HIGH); // Arka Planı Kapatmak İçin Low Yapmamız Gerekiyor 

  lcd.createChar(1,OzelKarakter0); // Özel Karakter Yaratma Fonksiyonu
  lcd.createChar(2,OzelKarakter1);
  lcd.createChar(3,OzelKarakter2);
  lcd.createChar(4,OzelKarakter3);

  Serial1.begin(9600); // Baud Rate 9600 Bps

  pinMode(BUZZER,OUTPUT); /* Çıkış İşlemleri */

 pinMode(LatchPin,OUTPUT);
 pinMode(DataPin,OUTPUT);
 pinMode(ClockPin,OUTPUT);

  Leds=0;  /* Başlangıçta Ledler Sönük Olsun */
  RegisterWrite();

 
}

void loop()
{
    LCDKEYPAD.BasilanTus = ButonOku(); // LCD Keypad Butonlarını Oku
    
   while(ButonDurum == 0) // 1.Pencere Başlangıç
      {
          LCDKEYPAD.BasilanTus = ButonOku(); // Buton Okuma Fonksiyonu
         if(LCDKEYPAD.BasilanTus == 4) // Eğer Enter Tuşuna Basılırsa İkinci Pencereye Geçmesini İstiyorum
    {
            while(LCDKEYPAD.BasilanTus == 4)  
            LCDKEYPAD.BasilanTus = ButonOku();
            delay(50);
            
            ButonDurum++;
            EKRAN1.imlecDurum++;
    }
        
    lcd.setCursor(0,0);           // 1.Pencere Bastıları
    lcd.print(" **Alarm Yok**");
    lcd.setCursor(0,1);
    lcd.write(3);
    lcd.setCursor(2,1);
    lcd.print("Alarm Kur");
    lcd.setCursor(12,1);          // Özel Karakter Bastısı
    lcd.write(EKRAN1.imlecDurum);
    delay(200);
  
    } // 1.Pencere Bitiş

  while(ButonDurum==1)  // 2.Pencere Başlangıç
  {
    
    LCDKEYPAD.BasilanTus = ButonOku();
    
 if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);        
        EKRAN2.imlecKaydirma++;
        

        if(EKRAN2.imlecKaydirma == 6)
      {
        EKRAN2.imlecKaydirma=7;
      }

      else if(EKRAN2.imlecKaydirma == 9)
      {
        EKRAN2.imlecKaydirma=8;
      }
    }


  else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {

          while(LCDKEYPAD.BasilanTus == 0)        
          LCDKEYPAD.BasilanTus = ButonOku();
          delay(50);          
          EKRAN2.imlecKaydirma--;
          

      if(EKRAN2.imlecKaydirma == 6)
      {
        EKRAN2.imlecKaydirma=5;
      }

        else if(EKRAN2.imlecKaydirma == 3)
      {
        EKRAN2.imlecKaydirma=4;
      }
    }

else if (LCDKEYPAD.BasilanTus == 1)  // Yukarı Değer Arttırma Koşulu
{
          while(LCDKEYPAD.BasilanTus == 1)        
          LCDKEYPAD.BasilanTus = ButonOku();
          delay(50); 
  
    if(EKRAN2.imlecKaydirma == 4)
    {
      EKRAN2.SaatOnlar++;

      if(EKRAN2.SaatOnlar == 3 )
      {
        EKRAN2.SaatOnlar=2;
      }

      else if (EKRAN2.SaatBirler>3 && EKRAN2.SaatOnlar==2)
      {
        EKRAN2.SaatOnlar=1;
      }
    }

    else if (EKRAN2.imlecKaydirma == 5)
    {
      EKRAN2.SaatBirler++;

      if(EKRAN2.SaatBirler==10)
      {
        EKRAN2.SaatBirler=9;
      }

      else if(EKRAN2.SaatOnlar == 2 && EKRAN2.SaatBirler>3)
      {
       EKRAN2.SaatBirler=3; 
      }
    }
      else if (EKRAN2.imlecKaydirma == 7)
      {
        EKRAN2.DakikaOnlar++;

        if(EKRAN2.DakikaOnlar>5)
        {
          EKRAN2.DakikaOnlar=5;
        }
      }

else if (EKRAN2.imlecKaydirma == 8)
{
  EKRAN2.DakikaBirler++;

          if(EKRAN2.DakikaBirler>9)
        {
          EKRAN2.DakikaBirler=9;
        }
}

}


else if (LCDKEYPAD.BasilanTus == 2)  // Aşağı Değer Azaltma Koşulu
{
          while(LCDKEYPAD.BasilanTus == 2)        
          LCDKEYPAD.BasilanTus = ButonOku();
          delay(50); 
  
    if(EKRAN2.imlecKaydirma == 4)
    {
      EKRAN2.SaatOnlar--;

      if(EKRAN2.SaatOnlar==255 )
      {
        EKRAN2.SaatOnlar=0;
      }
    }

    else if (EKRAN2.imlecKaydirma == 5)
    {
      EKRAN2.SaatBirler--;

      if(EKRAN2.SaatBirler==255)
      {
        EKRAN2.SaatBirler=0;
      }


    }
      else if (EKRAN2.imlecKaydirma == 7)
      {
        EKRAN2.DakikaOnlar--;

        if(EKRAN2.DakikaOnlar==255)
        {
          EKRAN2.DakikaOnlar=0;
        }
      }

else if (EKRAN2.imlecKaydirma == 8)
{
  EKRAN2.DakikaBirler--;

          if(EKRAN2.DakikaBirler==255)
        {
          EKRAN2.DakikaBirler=0;
        }
}

}

else if (LCDKEYPAD.BasilanTus == 4)  // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
        EKRAN2.imlecDurum++;
}

    
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print(EKRAN2.SaatOnlar);
    lcd.setCursor(5,0);
    lcd.print(EKRAN2.SaatBirler);
    lcd.setCursor(6,0);
    lcd.print(":");
    lcd.setCursor(7,0);
    lcd.print(EKRAN2.DakikaOnlar);
    lcd.setCursor(8,0);
    lcd.print(EKRAN2.DakikaBirler);
    lcd.setCursor(EKRAN2.imlecKaydirma,1);
    lcd.write(4);
    lcd.setCursor(10,0);
    lcd.write(EKRAN2.imlecDurum);
    delay(100);
  } // 2.Pencere Bitiş


  while(ButonDurum == 2) // Pazartesi Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorPazartesi=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorPazartesi=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) //  Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}
    delay(200);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Pazartesi");
    lcd.setCursor(EKRAN3.CursorPazartesi,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    
    
  } // Pazartesi Alarm Ekranı Bitiş Bloğu

 while(ButonDurum == 3) // Sali Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorSali=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorSali=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Sali");
    lcd.setCursor(EKRAN3.CursorSali,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
  } // Sali Alarm Ekranı Bitiş Bloğu

     while(ButonDurum == 4) // Carsamba Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorCarsamba=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorCarsamba=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4)
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Carsamba");
    lcd.setCursor(EKRAN3.CursorCarsamba,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
    
  }// Carsamba Alarm Ekranı Bitiş Bloğu
  
     while(ButonDurum == 5) // Persembe Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorPersembe=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorPersembe=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Persembe");
    lcd.setCursor(EKRAN3.CursorPersembe,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
    
  }// Persembe Alarm Ekranı Bitiş Bloğu


       while(ButonDurum == 6) // Cuma Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorCuma=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorCuma=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Cuma");
    lcd.setCursor(EKRAN3.CursorCuma,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
    
  }// Cuma Alarm Ekranı Bitiş Bloğu


       while(ButonDurum == 7) // Cumartesi Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorCumartesi=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorCumartesi=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Cumartesi");
    lcd.setCursor(EKRAN3.CursorCumartesi,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
    
  }// Cumartesi Alarm Ekranı Bitiş Bloğu


       while(ButonDurum == 8) // Pazar Alarm Kurma Ekranı 
  {

     LCDKEYPAD.BasilanTus = ButonOku();
     if(LCDKEYPAD.BasilanTus == 0) // Seçme İmlecini Sağa Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 0)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
         EKRAN3.CursorPazar=10;
    }

      else  if(LCDKEYPAD.BasilanTus == 3) // Seçme İmlecini Sola Kaydırma Koşulu
    {
        while(LCDKEYPAD.BasilanTus == 3)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        EKRAN3.CursorPazar=3;
    }

    else if (LCDKEYPAD.BasilanTus == 4) // Enter Tuşuna Basılırsa Pencere Atla
{
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        ButonDurum++;
}

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Pazar");
    lcd.setCursor(EKRAN3.CursorPazar,1);
    lcd.write(3);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(11,1);
    lcd.write(2);
    delay(200);
    
  }// Pazar Alarm Ekranı Bitiş Bloğu

  while(ButonDurum==9) // Son LCD Ekranı Ayarların Set Edildiğini Belirten Ekran
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Alarm  Kuruldu");
    lcd.setCursor(5,1);
    lcd.print(EKRAN2.SaatOnlar);
    lcd.setCursor(6,1);
    lcd.print(EKRAN2.SaatBirler);
    lcd.setCursor(7,1);
    lcd.print(":");
    lcd.setCursor(8,1);
    lcd.print(EKRAN2.DakikaOnlar);
    lcd.setCursor(9,1);
    lcd.print(EKRAN2.DakikaBirler);
    delay(200);
    
    ButonDurum++;    
  }

   /*Elde Edilen Bilgilerin İşlenmesi*/

    Saat = (EKRAN2.SaatOnlar*10)+(EKRAN2.SaatBirler);           
    Dakika = (EKRAN2.DakikaOnlar*10)+(EKRAN2.DakikaBirler);

  if(EKRAN3.CursorPazartesi == 3)
  {
    GunDizisi[0]=0;
  }

  else
  {
    GunDizisi[0]=1;
  }

    if(EKRAN3.CursorSali == 3)
  {
    GunDizisi[1]=0;
  }

  else
  {
    GunDizisi[1]=1;
  }

    if(EKRAN3.CursorCarsamba == 3)
  {
    GunDizisi[2]=0;
  }

  else
  {
    GunDizisi[2]=1;
  }

    if(EKRAN3.CursorPersembe == 3)
  {
    GunDizisi[3]=0;
  }

  else
  {
    GunDizisi[3]=1;
  }

    if(EKRAN3.CursorCuma == 3)
  {
    GunDizisi[4]=0;
  }

  else
  {
    GunDizisi[4]=1;
  }

    if(EKRAN3.CursorCumartesi == 3)
  {
    GunDizisi[5]=0;
  }

  else
  {
    GunDizisi[5]=1;
  }

    if(EKRAN3.CursorPazar == 3)
  {
    GunDizisi[6]=0;
  }

  else
  {
    GunDizisi[6]=1;
  }

/*Elde Edilen Bilgilerin Slave'e Gönderilmesi*/

  if(ButonDurum==10)
  {
    Serial1.write(Saat);
    Serial1.write(Dakika);
    for(int X=0; X<7; X++)
    {
     Serial1.write(GunDizisi[X]); 
    }
    ButonDurum++;
  }

     if(Serial1.available()>0)  // Slave'den Bilgi Gelirse Bunu Değişkenlere Aktar
  {
      GelenData = Serial1.read();
      delay(100);        
  }

  if(GelenData==1) // Alarm Ekranı
  {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("-----ALARM-----");
      lcd.setCursor(4,1);
      lcd.write(3);
      lcd.setCursor(5,1);
      lcd.print("KAPAT");
      delay(200);

/*Butona Basılırsa Alarm Ekranından Çıkar Basmadığı Sürece İkaz Led Ve Buzzer Çalışır*/


      while(1)
      {
            LCDKEYPAD.BasilanTus = ButonOku();
           if (LCDKEYPAD.BasilanTus == 4)
        {
        while(LCDKEYPAD.BasilanTus == 4)    
        LCDKEYPAD.BasilanTus = ButonOku();
        delay(50);
        GelenData=0; 
        break;
        }

    yeniZaman = millis();

  if(yeniZaman-eskiZaman>100)
  {
  Leds=0;
  RegisterWrite();
  delay(GecikmeSuresi);

    for(int i=0; i<8; i++)
  {
    bitSet(Leds,i);
    RegisterWrite();
    delay(GecikmeSuresi);
  }

      for(int i=8; i>=0; i--)
  {
    bitClear(Leds,i);
    RegisterWrite();
    delay(GecikmeSuresi);
  }

  BuzzerAlarm();
    
  }
  eskiZaman=yeniZaman;

     }              
  }

/*Ayarlar Yapıldıktan Sonra Sürekli Ekranda Kalmasını İstediklerim*/

  if(GelenData==0) 
{
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Alarm  Kuruldu");
    lcd.setCursor(5,1);
    lcd.print(EKRAN2.SaatOnlar);
    lcd.setCursor(6,1);
    lcd.print(EKRAN2.SaatBirler);
    lcd.setCursor(7,1);
    lcd.print(":");
    lcd.setCursor(8,1);
    lcd.print(EKRAN2.DakikaOnlar);
    lcd.setCursor(9,1);
    lcd.print(EKRAN2.DakikaBirler);
    delay(200);
}

}

void BuzzerAlarm() // BUZZER İKAZI
{
    digitalWrite(BUZZER,HIGH);
    delay(300);
    digitalWrite(BUZZER,LOW);
    delay(300);
  
}

 void RegisterWrite() // KARASİMSEK İKAZI
    {
      digitalWrite(LatchPin,LOW);
      shiftOut(DataPin,ClockPin,LSBFIRST,Leds);
      digitalWrite(LatchPin,HIGH);
    }
