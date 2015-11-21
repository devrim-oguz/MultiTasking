//Yazan: Ali Devrim OĞUZ	Tarih: 21.10.2015

/*
Merhabalar, MultiTasking kütüphanesinin kullanım kılavuzuna hoş geldiniz!
Bu kılavuzda kütüphaneyi nasıl kullanacağınıza dair bilgiler ve örnekler bulunmaktadır.
Yaptığım yazım hatalarını mazur görün :-)
*/

#include <MultiTasking.h>  //Başlamadan önce kütüphanemizi eklemezsek olmaz ;)

TaskList gorevLst; //Kütüphaneyi kullanmak için ilk önce bir görev listesi oluşturmalıyız.
//Tabiki ismini istediğiniz gibi belirleyebilirsiniz. Burada verdiğimiz isim: "gorevLst".

/*
Bu kütüphanenin amacı arduino üzerinde aynı anda birden çok işlem gerçekleştirmeyi
kolay bir hale getirmektir. Arduino Uno donanımsal olarak aynı anda birden çok işlem yapabilme
kapasitesine sahip değildir. Bu kütüphane de arduinoya böyle bir özellik kazandırmaz ancak
birden fazla işlemi beraber yapmak istiyorsak bu işlemlerin aynı anda çalışması gerekli değildir.
Eğer bu işlemler yeterince hızlı bir şekilde peş peşe yapılırsa beraber çalışıyormuş gibi gözükürler.
Bu kütüphanenin amacı da bunu gerçekleştirmektir. Aynı işlemi kendi kodunuzu yazarak da yapabilirsiniz ancak
bu kütüphane bu işi çok kolay hale getiriyor!

Neyse lafı fazla uzatmadan örneklere başlayalım:
*/



//****************************************************************************************************************************

/*	!!!ÖNEMLİ!!! BURAYI OKUMADAN GEÇMEYİN!

	Bu kütüphaneyi kullanırken en önemli nokta kodunuzun hiçbir yerinde delay fonksiyonunu veya türevlerini kullanmamanızdır.
Böyle bir durumda kütüphanenin işleyişi bozulur ve bütün programlar delay bitene kadar beklemeye geçer. Delay fonksiyonuyla
yapılacak çoğu şey bu kütüphaneyi kullanarak da yapılabilir.

/Kütüphane çalışmaya başlamadan önce delay fonksiyonunu kullanabilirsiniz.\ 
\Böyle bir durumda kütüphane delay fonksiyonundan sonra çalışmaya başlar. /*/

//****************************************************************************************************************************



//Aynı anda yapmak istediğiniz işlemlerden birisi 13. Pine bağlı LED'i saniyede bir yakıp söndürmek olsun.
//İlk önce LED'in bağlı olduğu pini tutan bir değişken oluşturalım:
int LED = 13; //Bu değişken LED'in 13. pine bağlı olduğunu belirtir.

//Ayrıca LED'in geçerli halini tutacak bir değişken de oluşturmamız gerekli:
boolean lambaDurumu = 0;
/* Bu satırda lambaDurumu isimli boolean tipinde(true yada false) bir değişken oluşturduk ve başlangıç değerini
0(false) olarak belirledik. */

//Aynı anda yapmak istediğimiz bir diğer işlem de analog bir pinden okunan değeri Seri monitöre yazdırmak olsun.
//Bunun için bir de analog pinin hangisi olduğunu tutacak bir değişken oluşturalım:
int analogPin = A0; //Burada analog pin'i A0 olarak belirledik.

//Öncelikle gerekli ayarları yapmak için setup fonksiyonunu tanımlayalım.
void setup() {
	Serial.begin(9600); //Serial monitörle işimiz olduğu için öncelikle serial monitörü başlatalım.

	pinMode( LED, OUTPUT ); //LED isimli pini çıkış pini olarak ayarlayalım ki lambayı yakıp söndürebilelim.

	//Not: fonksiyon tanımları loop fonksiyonundan sonra yapılmıştır.

	gorevLst.setTimer( lambaDurumuDegistir, 1000, 0 ); /*Lambanın durumunu değiştirecek fonksiyonun
	saniyede bir çağırılması için bu fonksiyonun ismini, ne kadar sürede bir çalıştıracağımızı ve
	kaç kere çalıştıracağımızı kütüphanemize bildirelim. Buradaki lambaDurumu değiştir lambayı yakıp
	söndürecek fonksiyonu, "1000" yazan yer fonksiyonun kaç milisaniyede bir çağırılacağını
	(1000 Milisaniye = 1 Saniye), sondaki "0" ise fonksiyonun kaç kere çalıştırılacağını belirtmek için
	kullanılmıştır. Eğer bu değere 0 verilirse fonksiyon sonsuza dek(veya başka bir komut tarafından
	durduruluncaya kadar) çalışmaya devam edecektir. Buradan anlayacağımız şey lambaDurumuDegistir
	fonksiyonunun sonsuza kadar(veya durdurulana kadar) saniyede bir çalıştırılacağıdır.
	*/

	gorevLst.setTimer( analogPinOku, 500, 0 ); /*Burada da 500 milisaniyede bir sonsuza kadar çalışacak bir
	fonksiyon kütüphaneye bildiriliyor. Bu fonksiyon analog bir pinden aldığı değeri seri monitöre yazdıracak.*/

	gorevLst.addThread( serialKontrolEt ); /*Ayni zamanda sürekli çalışacak bir fonksiyon da tanımlayabiliriz.
	Bunun için addThread komutunu kullanarak fonksiyonun ismini vermemiz yeterli.*/

	gorevLst.setTimer( acikKalmaSuresiYaz, 5000, 10 ); /*Bir de arduino'nun kaç saniyedir açık kaldığını seri
	monitöre yazdıracak bir fonksiyon ekleyelim. Bu fonksiyon diğerlerinin aksine 10 kere çalışacak.*/

	gorevLst.startTasks(); /*Kütüphaneyi çalıştırmaya başlıyoruz. Artık setup içerisinde bu komuttan sonra
	kütüphanenin çalışması durdurulana kadar hiçbir komut çalışmaz. Bu fonksiyon temel olarak bir sonsuz döngüdür.*/
}

/*Loop yine normalde olduğu gibi sonsuza kadar çalışmaya devam edecektir. İçine başka kodlar yazabilirsiniz veya
çalışmasını durdurmak için removeThread(loop); komutunu verebilirsiniz. Bu örnekte loop fonksiyonu
çalışmaya devam edecektir.*/
void loop() {
}

//Setup ve loop fonksiyonlarını tanımladıktan sonra kendi fonksiyonlarımızı tanımlamaya geçebiliriz.
//--------------------------------------------------------------------------------------------------

//Lamba yakıp söndürme işlemini yapacak bir fonksiyon tanımlıyoruz:
void lambaDurumuDegistir () { /*Burada önemli olan nokta fonksiyonun hiçbir değer alıp döndürmemesidir.
	Bu durum bütün fonksiyonlar için geçerlidir.*/
	if( lambaDurumu == false ) { //Bu fonksiyon eğer lamba kapalı ise
		digitalWrite(LED, HIGH); //lambayı açıyor,
		lambaDurumu = 1; //Lamba durumunu 1(açık) olarak kaydediyoruz.
	}
	else { //Eğer lamba açık ise
		digitalWrite(LED, LOW); //Lambayı kapatıyor,
		lambaDurumu = 0; //Lamba durumunu 0(kapalı) olarak kaydediyoruz.
	}
} //İlk fonksiyonumuzun tanımı bu kadardı. Bu fonksiyon setup içerisinde saniyede bir çalışacak şekilde ayarlanmıştır.

void analogPinOku () { //Analog pini okuyup seri monitöre yazdıracak fonksiyonumuzu tanımlıyoruz.
	int analogDegeri = analogRead(analogPin); //Analog pinden okunan değeri bir int değişkeni içerisinde tutuyoruz.
	Serial.println(analogDegeri); //Sonra bu degeri seri monitöre yazdırıyoruz.
} //Bu kadar.

void serialKontrolEt () { /*Burada serialden bir değer geldiğinde ekrana o değerin ne olduğunu yazdıran bir fonksiyon
	tanımlıyoruz.*/
	if( Serial.available() > 0 ) { //Eğer serialden bir değer okunursa aşağıdakileri yap.4
		Serial.print("Serialden gonderilen deger: "); //Seri monitöre gönderilen değer için bir açıklama yaz.
		char seriDeger = Serial.read(); //Serialden değeri okuyup bir değişkene kaydet.
		Serial.write(seriDeger); //Bu değişkeni seri monitöre yazdır.
		Serial.println(""); //Alt satıra geç.
	}
}

void acikKalmaSuresiYaz () { //Burda arduinoyu başlattığımızdan beri geçen süreyi yazan bir fonksiyon tanımlıyoruz.
	Serial.print("Acik kalma suresi: "); //Verilen değer için tanım.
	Serial.println( millis() / 1000 ); /*Arduinonun açık kalma süresini seri monitöre yazdırıyoruz. Buradaki millis()
	fonksiyonu arduino açıldığından beri geçen zamanı milisaniye cinsinden verir. Bu değeri 1000'e bölersek arduino'nun
	kaç saniyedir açık durduğunu hesaplayabiliriz.*/
}

/* Fonksiyonların ayrıntıları:
	
	//======================================================================================================================
	--- addThread( fonksiyonİsmi ) ---
	Bu fonksiyon sürekli çalışacak bir fonksiyon eklemek için kullanılır.
	Verilen fonksiyonun imzası şu şekilde olmalıdır;
	void fonksiyonİsmi ( void )
	Eğer işlem başarılı olursa 1, diğer durumlarda 0 değeri döndürür.

	//======================================================================================================================
	--- setTimer( fonksiyonİsmi, calismaAraligi, calismaSayisi ) ---
	Bu fonksiyon belirli bir aralıkla(ve istenirse belirli bir miktarda) çalışacak bir fonksiyon eklemek için kullanılır.
	Verlien fonksiyonun imzası şu şekilde olmalıdır;
	void fonksiyonİsmi ( void )
	Eğer işlem başarılı olursa 1, diğer durumlarda 0 değeri döndürür.

	//======================================================================================================================
	--- startTasks() ---
	Kütüphanenin çalışmasını başlatır. Bu fonksiyondan sonra yazılan aynı scope içindeki komutlar kütüphanenin çalışması
	durdurulana kadar çalışmaz.

	//======================================================================================================================
	--- removeThread( fonksiyonİsmi ) ---
	Bu fonksiyon addThread tarafından önceden eklenmiş fonksiyonları kaldırmak için kullanılır.
	Eğer işlem başarılı olursa 1, diğer durumlarda 0 değeri döndürür.

	//======================================================================================================================
	--- killTimer( fonksiyonİsmi ) ---
	Bu fonksiyon setTimer tarafından önceden eklenmiş fonksiyonları kaldırmak için kullanılır.
	Eğer işlem başarılı olursa 1, diğer durumlarda 0 değeri döndürür.

	//======================================================================================================================
	--- isThreadRunning( fonksiyonİsmi ) ---
	Bu fonksiyon bir fonksiyonun daha önce addThread tarafından eklenip eklenmediğine bakar. Eğer fonksiyon eklenmiş ise 1,
	eklenmemiş ise 0 değeri döndürür.

	//======================================================================================================================
	--- isTimerRunning( fonksiyonİsmi ) ---
	Bu fonksiyon bir fonksiyonun daha önce setTimer tarafından eklenip eklenmediğine bakar. Eğer fonksiyon eklenmiş ise 1,
	eklenmemiş ise 0 değeri döndürür.

	//======================================================================================================================
	--- flushThreads() ---
	Bu fonksiyon addThread tarafından eklenmiş bütün fonksiyonları kaldırır. Buna loop() fonksiyonu da dahildir.

	//======================================================================================================================
	--- flushTimers() ---
	Bu fonksiyon setTimer tarafından eklenmiş bütün fonksiyonları kaldırır.

	//======================================================================================================================
	--- flushAll() ---
	Bu fonksiyon addThread ve setTimer tarafından eklenmiş bütün fonksiyonları kaldırır. Buna loop() fonksiyonu da dahildir.

	//**********************************************************************************************************************
	Performans ve ram kullanımı açısından kütüphanenin Thread ve Timerlar için sınırlaması şu şekildedir;

	Aynı anda çalışabilecek maksimum Thread sayısı: 50
	Aynı anda çalışabilecek maksimum Timer sayısı:	20

	Bu değerler kütüphanenin MultiTasking.h dosyası içerisinde şu şekilde belirtilmiştir;
	(Bu değerler MultiTasking.h dosyasının başında bulunabilir.)

	#define MAXIMUM_THREADS 50
	#define MAXIMUM_TIMERS	20

	Buradaki MAXIMUM_THREADS ve MAXIMUM_TIMERS değerlerini değiştirerek kendi ihtiyaçlarınıza uygun ayarlayabilirsiniz.

	Kütüphane varsayılan ayarlarda Arduino UNO üzerinde ortalama 529 Byte (%29) RAM kullanmaktadır.
	Arduino UNO için her thread RAM üzerinde tahmini 3 byte, her timer tahmini 9 byte yer harcamaktadır.

	!!!KÜTÜPHANEYİ KULLANDIĞINIZ İÇİN TEŞEKKÜRLER!!!
	*/
