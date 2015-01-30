#include <stdio.h>
#include <stdlib.h>

struct ogrenciler // ogrenci bilgilerinin tutuldugu veri yapisi.
{
    int ogr_no;
    char ad_soyad[31];
    int sinif;
    int aldigi_kredi_say;
    float not_ort;
    struct ogrenciler *sonraki;
};

struct dersin_ogrencisi // Dersler ile dersi alan ogrencilerin baglantisinin kuruldugu veri yapisi.
{
    int ogr_no;
    int ogr_notu;
    struct dersin_ogrencisi *noya_gore_sonraki;
    struct dersin_ogrencisi *nota_gore_sonraki;
    struct dersin_ogrencisi *nota_gore_onceki;
};
struct dersler // ders bilgilerinin tutuldugu veri yapisi.
{
    int ders_kodu;
    char ders_adi[26];
    int kredisi;
    int dersi_alan_ogr_say;
    float dersin_not_ort;
    struct dersin_ogrencisi liste_basi; // dersi alan ogrencinin eklendigi baglı listelerin basi.
};

int menu();
void ogrenci_ekleme(struct ogrenciler  * *ilk, struct ogrenciler  *bir_ogrenci );
struct ogrenciler *noya_gore_ogr_ara(struct ogrenciler **ilk ,int);
int ders_kodu_belirle(struct dersler **,int );
void dairesel_tek_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *ogrenci);
void dairesel_cift_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *ogrenci);
struct dersin_ogrencisi *noya_gore_bagli_listede_ara(struct dersin_ogrencisi *liste_basi,int no);
void cift_baglidan_cikar(struct dersin_ogrencisi *cikarilacak);
struct dersin_ogrencisi * tek_baglidan_cikar(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi * ogrenci);
struct ogrenciler * ogrencilerden_cikar(struct ogrenciler **ilk,int ogr_no);
void  bir_dnmn_derslerinin_listelenmesi(struct dersler*ptr_dizisi[80]);
void   bir_sinifta_okuyan_ogr_listeleme(struct ogrenciler *yapi_dizisi[100]);
void kucukten_buyuge_listele(struct dersin_ogrencisi *liste_basi,struct ogrenciler *yapi_dizisi[]);
void duruma_gore_cift_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *yeni);
int sayi_al(int,int);

int main()
{
    int i,secenek,hash,ders_kodu,donem,ogr_no,not_baraji,sayac=0,yeni_notu;
    char devam;
    struct ogrenciler *yapi_dizisi[100]= {NULL}; // odevde yapi dizisi diye bahsedilen, ama aslında bir ptr dizisi olan dizi.
    struct dersler *ptr_dizisi[80]= {NULL};
    struct ogrenciler *bir_ogrenci;
    struct dersin_ogrencisi *ogrenci;
    struct dersin_ogrencisi *gecici;

    secenek=menu();
    while (secenek!=13)
    {
        switch(secenek)
        {
        case 1: // Odevin 1. secenegi.
            bir_ogrenci=malloc(sizeof(struct ogrenciler));
            bir_ogrenci->not_ort=0;
            bir_ogrenci->aldigi_kredi_say=0;
            printf("Ogrenci numarasini giriniz:\n");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;

            if(noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no)==NULL)
            {
                bir_ogrenci->ogr_no=ogr_no;
                printf("Ogrencini adini ve soyadini giriniz\n");
                fflush(stdin);
                gets(bir_ogrenci->ad_soyad);
                printf("Ogrencinin sinifini giriniz\n");
                scanf("%d",&(bir_ogrenci->sinif));
                ogrenci_ekleme(&yapi_dizisi[hash],bir_ogrenci); // ogrencinin tek bagli listede uygun yere eklenmesi.
                printf("Ogrenci eklendi\n");
            }
            else printf("bu numaraya sahip bir ogrenci vardir\n");

            break;
        case 2: // Odevin 2. secenegi.
            printf("Eklemek istediginiz dersin donemini giriniz\n");
            donem=sayi_al(1,8);
            struct dersler *bir_ders=malloc(sizeof(struct dersler));
            (bir_ders->liste_basi).nota_gore_onceki=&(bir_ders->liste_basi); // ders olusturulduktan sonra liste basi kendini gösteriyor.
            (bir_ders->liste_basi).nota_gore_sonraki=&(bir_ders->liste_basi);
            (bir_ders->liste_basi).noya_gore_sonraki=&(bir_ders->liste_basi);
            bir_ders->dersi_alan_ogr_say=0;
            bir_ders->dersin_not_ort=0;

            ders_kodu=ders_kodu_belirle(ptr_dizisi,donem);
            bir_ders->ders_kodu=ders_kodu;

            printf("Dersin kodu:%d\n",bir_ders->ders_kodu);
            printf("Dersin adini giriniz\n");
            fflush(stdin);
            gets(bir_ders->ders_adi);
            printf("Dersin kredisini giriniz\n");
            scanf("%d",&(bir_ders->kredisi));
            ptr_dizisi[ders_kodu-10]=bir_ders;
            printf("Ekleme islemi tamamlandi\n");
            break;

        case 3: // Odevin 3. secenegi.
            printf("Ogrencileri kayitlamak istediginiz dersin kodunu giriniz\n");
            ders_kodu=sayi_al(10,89);
            if(ptr_dizisi[ders_kodu-10]!=NULL) // girilen dersin listede olup olmadigini kontrol eder.
            {
                do
                {
                    printf("Bu derse kayitlamak istediginiz ogrencinin numarasini giriniz\n");
                    ogr_no=sayi_al(1,10000);
                    hash=(ogr_no-1)/100;
                    bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no); // tek bagli listede ogrenci aranir.
                    if(bir_ogrenci!=NULL)
                    {
                        ogrenci=noya_gore_bagli_listede_ara(&(ptr_dizisi[ders_kodu-10]->liste_basi),ogr_no);
                        if(ogrenci==NULL)
                        {
                            ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say++;//bir_ders yapisinin içinde bulunan dersi alan ogr sayisi artiriliyor.

                            ogrenci=malloc((sizeof(struct dersin_ogrencisi)));
                            printf("Ogrencinin notunu giriniz\n");
                            scanf("%d",&(ogrenci->ogr_notu));

                            ptr_dizisi[ders_kodu-10]->dersin_not_ort=(((ptr_dizisi[ders_kodu-10]->dersin_not_ort)*(ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say-1))+ogrenci->ogr_notu)/ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say;

                            ogrenci->ogr_no=ogr_no;
                            bir_ogrenci->not_ort=((ptr_dizisi[ders_kodu-10]->kredisi*ogrenci->ogr_notu)+(bir_ogrenci->not_ort*bir_ogrenci->aldigi_kredi_say))/(bir_ogrenci->aldigi_kredi_say+bir_ders->kredisi); //ogrencinin bu dersten aldigi not alýndýktan sonra genele not ortalamasi hesaplaniyor.
                            bir_ogrenci->aldigi_kredi_say+=ptr_dizisi[ders_kodu-10]->kredisi;   //ogrencinin aldigi kredi sayisi da artiriliyor.
                            dairesel_tek_bagliya_ekle(&(ptr_dizisi[ders_kodu-10]->liste_basi),ogrenci); // dairesel listelere ogrenci eklenir.
                            dairesel_cift_bagliya_ekle(&(ptr_dizisi[ders_kodu-10]->liste_basi),ogrenci);
                            printf("Ogrenci eklendi\n");

                        }
                        else printf("Bu ogrenci bu derse zaten kayitli\n");
                    }
                    else printf("Bu numaraya sahip bir ogrenci yoktur\n");

                    do
                    {
                        printf("Devam etmek istiyor musunuz(e/E/h/H):\n");
                        scanf(" %c",&devam);
                    }while(devam!='e' && devam!='E' && devam!='h'  && devam!='H' );
                }
                while(devam=='e' || devam=='E');
            }
            else printf("Bu koda sahip bir ders yok\n");
            break;

        case 4: // Odevin 4. secenegi.
            ogrenci=malloc(sizeof(struct dersin_ogrencisi));
            printf("Dersin kodunu giriniz\n");
            ders_kodu=sayi_al(10,89);
            if(ptr_dizisi[ders_kodu-10]!=NULL)
            {
                printf("ogrenci numarasini giriniz\n");
                ogr_no=sayi_al(1,10000);
                hash=(ogr_no-1)/100;
                bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);

                if(bir_ogrenci!=NULL)
                {   // ogrencii numarasina göre bagli listede arama yapiliyor.
                    ogrenci=noya_gore_bagli_listede_ara(&(ptr_dizisi[ders_kodu-10]->liste_basi),ogr_no);
                    if(ogrenci!=NULL)
                    {
                        printf("Ogrencinin yeni notunu giriniz\n");
                        scanf("%d",&yeni_notu); // dersin not ort ve ogrencinin not ort degistirilir.
                        ptr_dizisi[ders_kodu-10]->dersin_not_ort=((ptr_dizisi[ders_kodu-10]->dersin_not_ort*ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say)-(ogrenci->ogr_notu)+(yeni_notu))/ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say;
                        bir_ogrenci->not_ort=((bir_ogrenci->not_ort*bir_ogrenci->aldigi_kredi_say)-(ogrenci->ogr_notu*ptr_dizisi[ders_kodu-10]->kredisi)+(yeni_notu*ptr_dizisi[ders_kodu-10]->kredisi))/bir_ogrenci->aldigi_kredi_say;
                        ogrenci->ogr_notu=yeni_notu; // ogrencinin yeni notu alinir ve cift bagli listeye eklenir.
                        duruma_gore_cift_bagliya_ekle(&(ptr_dizisi[ders_kodu-10]->liste_basi),ogrenci);
                        hash=( bir_ogrenci->ogr_no-1)/100;

                        printf("Ogrencinin notu guncellendi\n");
                    }
                    else printf("Ogrenci bu dersi almamistir\n");
                }
                else printf("Boyle bir ogrenci yok\n");
            }
            else printf("Bu koda sahip bir ders yok\n");
            break;

        case 5: // Odevin 5. secenegi.
            bir_ogrenci=malloc(sizeof(struct ogrenciler));
            ogrenci=malloc(sizeof(struct dersin_ogrencisi));
            printf("ogrenci numarasini girinz\n");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;
            bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);

            if(bir_ogrenci!=NULL)
            {
                for(i=0; i<80; i++)
                {
                    if(ptr_dizisi[i]!=NULL)
                    {
                        ogrenci=noya_gore_bagli_listede_ara(&(ptr_dizisi[i]->liste_basi),ogr_no);

                        if(ogrenci!=NULL)
                        {
                            cift_baglidan_cikar(ogrenci); // silmek istedigimiz ogrenciyi cift bagli listeden cikariyor.
                            ogrenci=tek_baglidan_cikar(&(ptr_dizisi[i]->liste_basi),ogrenci);
                            bir_ogrenci=ogrencilerden_cikar(&yapi_dizisi[hash],ogr_no); // hash li isaretci dizisinden cikariliyor.

                            ptr_dizisi[i]->dersi_alan_ogr_say--; // dersin genel not ortalamasi, silinen ogrencinin notu cikarilarak hesaplaniyor.
                            ptr_dizisi[i]->dersin_not_ort=(((ptr_dizisi[i]->dersin_not_ort)*(ptr_dizisi[i]->dersi_alan_ogr_say+1))-ogrenci->ogr_notu)/ptr_dizisi[i]->dersi_alan_ogr_say;
                            printf("Ogrenci kayitli oldugu derslerden silindi\n");
                             free(ogrenci);
                        }
                    }
                }
                free(bir_ogrenci);
                printf("Ogrenci silindi\n");
            }
            else printf("Boyle bir ogrenci yok\n");

            break;
        case 6: // Odevin 6. secenegi.
            printf("Ders kodunu giriniz:\n");
            ders_kodu=sayi_al(10,89);
            if(ptr_dizisi[ders_kodu-10]!=NULL)
            {

                printf("Ders Kodu  Ders Adi         Kredi  Ogrenci Say     Not Ort\n");
                printf("---------  ---------------  ----  ---------        ------\n");
                printf("%-9d  %-15s  %-5d  %-9d       %.2f\n",ptr_dizisi[ders_kodu-10]->ders_kodu,ptr_dizisi[ders_kodu-10]->ders_adi,ptr_dizisi[ders_kodu-10]->kredisi,ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say,ptr_dizisi[ders_kodu-10]->dersin_not_ort);

                printf("Dersi alan ogrenciler:\n");
                printf("Ogr No    Ad Soyad    Sinif   Notu\n");
                printf("---------  ---------  ----   ------\n");
                if((ptr_dizisi[ders_kodu-10]->liste_basi).noya_gore_sonraki != &(ptr_dizisi[ders_kodu-10]->liste_basi))
                {
                    gecici=(ptr_dizisi[ders_kodu-10]->liste_basi).noya_gore_sonraki;
                    while(gecici!=&(ptr_dizisi[ders_kodu-10]->liste_basi))
                    {
                        ogrenci= noya_gore_bagli_listede_ara(&(ptr_dizisi[ders_kodu-10]->liste_basi),gecici->ogr_no);
                        ogr_no=ogrenci->ogr_no;
                        hash=(ogr_no-1)/100;
                        bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);
                        printf("%-9d  %-7s  %-6d    %d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,ogrenci->ogr_notu);
                        gecici=gecici->noya_gore_sonraki;
                    }
                }
                else
                    printf("Bu dersi alan hic bir ogrenci yoktur!!!\n");
            }
            else
                printf("Bu kodlu bir ders yoktur!!!\n");
            break;
        case 7: // Odevin 7. secenegi.
            sayac=0;
            bir_ogrenci=malloc(sizeof(struct ogrenciler));
            printf("Ders kodunu giriniz:\n");
            ders_kodu=sayi_al(10,89);
            if(ptr_dizisi[ders_kodu-10]!=NULL)
            {
                printf("Not barajini giriniz:\n");
                scanf("%d",&not_baraji);

                if((ptr_dizisi[ders_kodu-10]->liste_basi).nota_gore_sonraki != &(ptr_dizisi[ders_kodu-10]->liste_basi))
                {
                    printf("Notu yuksek olan ogrenciler:\n");
                    printf("Ogr No    Ad Soyad     Sinif   Notu\n");
                    printf("---------  ---------  ----   ------\n");
                    gecici=(ptr_dizisi[ders_kodu-10]->liste_basi).nota_gore_onceki; // bagli listede artan sirada tutulan notlari azalan sirada yazdirabilmek icin geriye dogru arama yaptiriyoruz.
                    while(gecici!=&(ptr_dizisi[ders_kodu-10]->liste_basi))
                    {
                        if(gecici->ogr_notu>not_baraji)
                        {
                            sayac++;
                            ogr_no=gecici->ogr_no;
                            hash=(ogr_no-1)/100;
                            bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);
                            printf("%-9d  %-7s  %-6d    %d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,gecici->ogr_notu);
                        }
                        gecici=gecici->nota_gore_onceki;
                    }
                    printf("Notu %d nin ustunde olan ogrenci sayisi:%d \t Yuzdesi:%.2f\n",not_baraji,sayac,(float)((sayac*100/(ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say))));
                }
                else
                    printf("Bu kodlu dersli alan bir ogrenci yoktur.\n");
            }
            else
                printf("Bu kodlu bir ders yoktur\n");
            break;
        case 8: // Odevin 8. secenegi.
            sayac=0;
            printf("Ders kodunu giriniz:\n");
            ders_kodu=sayi_al(10,89);
            if(ptr_dizisi[ders_kodu-10]!=NULL)
            {
                printf("Not barajini giriniz:\n");
                scanf("%d",&not_baraji);

                if((ptr_dizisi[ders_kodu-10]->liste_basi).nota_gore_sonraki != &(ptr_dizisi[ders_kodu-10]->liste_basi))
                {
                    printf("Notu dusuk  olan ogrenciler:\n");
                    printf("Ogr No     Ad Soyad    Sinif   Notu\n");
                    printf("---------  ----------  ----   ------\n");
                    gecici=(ptr_dizisi[ders_kodu-10]->liste_basi).nota_gore_sonraki; // bagli listede artan sirada tutulan notlari ileriye dogru arama yaptiriyoruz.
                    while(gecici!=&(ptr_dizisi[ders_kodu-10]->liste_basi))
                    {
                        if(gecici->ogr_notu<not_baraji)
                        {
                            sayac++;
                            ogr_no=gecici->ogr_no;
                            hash=(ogr_no-1)/100;
                            bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);
                            printf("%-9d  %-7s  %-9d    %d\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,gecici->ogr_notu);
                        }
                        gecici=gecici->nota_gore_sonraki;
                    }
                    printf("Notu %d nin altinda olan ogrenci sayisi:%d \t Yuzdesi:%.2f\n",not_baraji,sayac,(float)((sayac*100/(ptr_dizisi[ders_kodu-10]->dersi_alan_ogr_say))));
                }
                else
                    printf("Bu kodlu dersli alan bir ogrenci yoktur.\n");
            }
            else
                printf("Bu kodlu bir ders yoktur\n");
            break;
        case 9: // Odevin 9. secenegi.
            bir_dnmn_derslerinin_listelenmesi(ptr_dizisi);
            break;
        case 10: // Odevin 10. secenegi.

            printf("Goruntulemek istediginiz ogrencinin numarasini giriniz\n");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;
            bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);
            if(bir_ogrenci!=NULL)
            {
                printf("Ogr No    Ad Soyad        Sinif  Kredi Say     Not Ort\n");
                printf("---------  -------------  ----  ---------        ------\n");
                printf("%-9d  %-15s  %-5d  %-9d    %.2f\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,bir_ogrenci->aldigi_kredi_say,bir_ogrenci->not_ort);//2ye boluyo.
            }
            else printf("Bu numaraya sahip bir ogrenci yoktur\n");

            break;
        case 11: // Odevin 11. secenegi.
            printf("Goruntulemek istediginiz ogrencinin numarasini giriniz\n");
            ogr_no=sayi_al(1,10000);
            hash=(ogr_no-1)/100;
            bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);
            if(bir_ogrenci!=NULL)
            {
                printf("Ogr No    Ad Soyad        Sinif  Kredi Say     Not Ort\n");
                printf("---------  -------------  ----  ---------        ------\n");
                printf("%-9d  %-15s  %-5d  %-9d    %.2f\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,bir_ogrenci->aldigi_kredi_say,bir_ogrenci->not_ort);
                printf("Aldigi dersler:\n");
                printf("Ders kodu  Ders adi Kredi Notu\n");
                printf("---------  -------- ----- ------\n");

                for(i=0; i<80; i++)
                {
                    if(ptr_dizisi[i]!=NULL)
                    {
                        ogrenci=noya_gore_bagli_listede_ara(&(ptr_dizisi[i]->liste_basi),ogr_no);
                        if(ogrenci!=NULL)
                        {
                            printf("%-9d  %-9s  %-5d %-9d\n",ptr_dizisi[i]->ders_kodu,ptr_dizisi[i]->ders_adi,ptr_dizisi[i]->kredisi,ogrenci->ogr_notu);
                        }
                    }
                }
            }
            else printf("Bu numaraya sahip bir ogrenci yoktur\n");
            break;
        case 12: // Odevin 12. secenegi.
            bir_sinifta_okuyan_ogr_listeleme(yapi_dizisi);
            break;
        }
        secenek=menu();
    }

    return 0;
}
int sayi_al(int alt,int ust) // verilerin istenilen aralikta girilmesini kontrol eder.
{
    int sayi;

    scanf("%d",&sayi);
    while(sayi<alt || sayi>ust)
    {
        printf("Hatali giris yaptiniz,tekrar giriniz:");
        scanf("%d",&sayi);
    }
    return sayi;
}

int menu()
{
    int secim;
    printf("*********************Ogrenci Ders Takip Sistemi-2*************************\n"
           "1-Yeni bir ogrencinin eklenmesi\n"
           "2-Yeni bir dersin eklenmesi\n"
           "3-Bir dersi alan ogrencilerin notlarinin eklenmesi\n"
           "4-Bir dersi alan bir ogrencinin notunun guncellenmesi\n"
           "5-Bir ogrecinin silinmesi\n"
           "6-Bir dersin bilgilerinin ve o dersi alan ogrencilerin listelenmesi\n"
           "7-Bir dersi alan ogrencilerden, notu belirli bir notun ustunde olan ogrencilerin listelenmesi\n"
           "8-Bir dersi alan ogrencilerden, notu belirli bir notun altinda olan ogrencilerin listelenmesi\n"
           "9-Bir donemin derslerinin listelenmesi\n"
           "10-Bir ogrencinin bilgilerinin listelenmesi\n"
           "11-Bir ogrencinin bilgilerinin ve aldigi derslerin listelenmesi\n"
           "12-Bir sinifta okuyan ogrencilerin listelenmesi\n"
           "13-Cikis\n"
           "*************************************************************************\n"
          );

    printf("seciminizi giriniz:");
    scanf("%d",&secim);
    printf("-----------------------------\n");
    return secim;
}

void ogrenci_ekleme(struct ogrenciler **ilk,struct ogrenciler  *bir_ogrenci) // tek bali listeye eleman ekleme.
{
    struct ogrenciler *gecici;
    struct ogrenciler *onceki;

    if(*ilk==NULL)
    {
        bir_ogrenci->sonraki=NULL;
        *ilk=bir_ogrenci;

    }
    else if((*ilk)->ogr_no>bir_ogrenci->ogr_no)
    {
        bir_ogrenci->sonraki=*ilk;
        *ilk=bir_ogrenci;
    }
    else
    {
        onceki=*ilk;
        gecici=(*ilk)->sonraki;
        while(gecici!=NULL && gecici->ogr_no<bir_ogrenci->ogr_no)
        {
            onceki=gecici;
            gecici=gecici->sonraki;
        }
        bir_ogrenci->sonraki=gecici;
        onceki->sonraki=bir_ogrenci;
    }
}

struct ogrenciler *noya_gore_ogr_ara(struct ogrenciler **ilk,int ogr_no) // tek bagli listede numaraya göre ogrenci arama.
{
    struct ogrenciler *gecici;
    if (*ilk!=NULL)
    {
        gecici=*ilk;
        while(gecici!=NULL && gecici->ogr_no<=ogr_no)
        {
            if(gecici->ogr_no==ogr_no)
            {
                return gecici;
            }
            gecici=gecici->sonraki;
        }
    }

    return NULL;
}

int ders_kodu_belirle(struct dersler **ptr_dizisi,int donem) // dersin doneminden kodunun hesaplanmasi.
{
    int i,ders_kodu;
    for(i=donem*10-10; i<donem*10+10; i++)
    {
        if(ptr_dizisi[i]==NULL)
        {
            ders_kodu=i+10;
            break;
        }
    }
    return ders_kodu;
}

void dairesel_tek_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *ogrenci)
{
    struct dersin_ogrencisi *gecici, *onceki;
    onceki=liste_basi;
    gecici=liste_basi->noya_gore_sonraki;
    while(gecici!=liste_basi && gecici->ogr_no <ogrenci->ogr_no)
    {
        onceki=gecici;
        gecici=gecici->noya_gore_sonraki;
    }
    ogrenci->noya_gore_sonraki=gecici;
    onceki->noya_gore_sonraki=ogrenci;
}

void dairesel_cift_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *ogrenci)
{
    struct dersin_ogrencisi *gecici=liste_basi->nota_gore_sonraki;
    while (gecici!=liste_basi && gecici->ogr_notu<ogrenci->ogr_notu)
        gecici=gecici->nota_gore_sonraki;
    ogrenci->nota_gore_onceki=gecici->nota_gore_onceki;
    ogrenci->nota_gore_sonraki=gecici;
    gecici->nota_gore_onceki->nota_gore_sonraki=ogrenci;
    gecici->nota_gore_onceki=ogrenci;

}
void duruma_gore_cift_bagliya_ekle(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *yeni)
{ // ogrencinin notu guncellendikten sonra cift bagli listede uygun yere yerleştiriliyor.
    struct dersin_ogrencisi *gecici;
    if(yeni->nota_gore_onceki!=liste_basi && yeni->nota_gore_onceki->ogr_notu>yeni->ogr_notu)// dugumun nereye tasinicaginin saptanmasi
    {
        gecici=yeni->nota_gore_onceki;
        while(gecici->nota_gore_onceki!=liste_basi && yeni->ogr_notu<gecici->nota_gore_onceki->ogr_notu)
            gecici=gecici->nota_gore_onceki;
        cift_baglidan_cikar(yeni);

        yeni->nota_gore_sonraki=gecici;//yeni baglarin kurulmasi
        yeni->nota_gore_onceki=gecici->nota_gore_onceki;
        gecici->nota_gore_onceki->nota_gore_sonraki=yeni;
        gecici->nota_gore_onceki=yeni;
    }
    else if(yeni->nota_gore_sonraki!=liste_basi && yeni->nota_gore_sonraki->ogr_notu<yeni->ogr_notu)
    {
        gecici=yeni->nota_gore_sonraki;
        while(yeni->nota_gore_sonraki!=liste_basi && yeni->ogr_notu>gecici->nota_gore_sonraki->ogr_notu)
            gecici=gecici->nota_gore_sonraki;
        cift_baglidan_cikar(yeni);

        yeni->nota_gore_sonraki=gecici->nota_gore_sonraki;//yeni baglarin kurulmasi
        yeni->nota_gore_onceki=gecici;
        gecici->nota_gore_sonraki->nota_gore_onceki=yeni;
        gecici->nota_gore_sonraki=yeni;
    }
}

struct dersin_ogrencisi * tek_baglidan_cikar(struct dersin_ogrencisi *liste_basi,struct dersin_ogrencisi *ogrenci)//
{ // ogrenci silinmeden once tek bagli listeden cikariliyor.
    struct dersin_ogrencisi *onceki, *gecici;
    onceki=liste_basi;
    gecici=liste_basi->noya_gore_sonraki;
    while(gecici!=liste_basi && gecici->ogr_no<ogrenci->ogr_no)
    {
        onceki=gecici;
        gecici=gecici->noya_gore_sonraki;
    }
    if(gecici!=liste_basi && gecici->ogr_no==ogrenci->ogr_no)
    {
        onceki->noya_gore_sonraki=gecici->noya_gore_sonraki;
        return gecici;
    }
    return NULL;

}
void cift_baglidan_cikar(struct dersin_ogrencisi *cikarilacak)
{ // ogrenci silinmeden once cift bagli listeden cikariliyor.
    cikarilacak->nota_gore_onceki->nota_gore_sonraki=cikarilacak->nota_gore_sonraki;
    cikarilacak->nota_gore_sonraki->nota_gore_onceki=cikarilacak->nota_gore_onceki;
}

struct dersin_ogrencisi * noya_gore_bagli_listede_ara(struct dersin_ogrencisi *liste_basi,int no)
{ // bir dersi alan ogrenci ogrenciler arasinda araniyor.
    struct dersin_ogrencisi *gecici;

    gecici=liste_basi->noya_gore_sonraki;
    while(gecici!=liste_basi && gecici->ogr_no<no)
    {
        gecici=gecici->noya_gore_sonraki;
    }
    if(gecici!=liste_basi && gecici->ogr_no==no)
    {
        return gecici;
    }

    return NULL;
}

struct ogrenciler * ogrencilerden_cikar(struct ogrenciler **ilk,int ogr_no)
{ // ogrencinin hashli diziden silinmesi icin cikarilir.
    struct ogrenciler *onceki, *gecici;

    if((*ilk)->ogr_no==ogr_no)
    {
        gecici=*ilk;
        *ilk=(*ilk)->sonraki;
        return gecici;
    }
    else
    {
        onceki=*ilk;
        gecici=(*ilk)->sonraki;
        while(gecici!=NULL && gecici->ogr_no<ogr_no)
        {
            onceki=gecici;
            gecici=gecici->sonraki;
        }

        if(gecici!=NULL && gecici->ogr_no==ogr_no)
        {
            onceki->sonraki=gecici->sonraki;
            return gecici;
        }
    }
    return NULL;
}

void  bir_dnmn_derslerinin_listelenmesi(struct dersler*ptr_dizisi[])
{
    int i,kredi_top,donem,bulundu=0;
    printf(" Donemi giriniz:\n");
    donem=sayi_al(1,8);

    printf("Ders Kodu  Ders Adi         Kredi  Ogrenci Say     Not Ort\n");
    printf("---------  ---------------  ----  ---------        ------\n");
    kredi_top=0;

    for(i=donem*10-10; i<=((donem*10)-1); i++)
    {
        if(ptr_dizisi[i]!=NULL)
        {
            bulundu=1;
            kredi_top+=ptr_dizisi[i]->kredisi;
            printf("%-9d  %-15s  %-5d  %-9d       %.2f\n",i+10,ptr_dizisi[i]->ders_adi,ptr_dizisi[i]->kredisi,ptr_dizisi[i]->dersi_alan_ogr_say,ptr_dizisi[i]->dersin_not_ort);
        }
    }
    printf("Derslerin kredi toplami:%d\n",kredi_top);
    if(bulundu!=1)
        printf("Bu donem de alinan bir ders yoktur\n");
}
void   bir_sinifta_okuyan_ogr_listeleme(struct ogrenciler *yapi_dizisi[])//12.fonk
{
    struct ogrenciler *gecici;
    int i,ogr_say=0,altmis_alti_say=0,bulundu=0,sinif;
    float not_top=0;
    printf("Ogrencilerini gormek istediginiz sinifi giriniz:\n");
    scanf("%d",&sinif);

    printf("Ogr No    Ad Soyad        Sinif  Kredi Say     Not Ort\n");
    printf("---------  -------------  ----  ---------        ------\n");

    for(i=0; i<100; i++)
    {
        if(yapi_dizisi[i]!=NULL)
        {
            gecici=yapi_dizisi[i];
            while(gecici!=NULL)
            {
                if(  (gecici->sinif ==sinif))
                {
                    printf("%-9d  %-15s  %-5d  %-9d       %.2f\n",gecici->ogr_no,gecici->ad_soyad,gecici->sinif,gecici->aldigi_kredi_say,gecici->not_ort);
                    ogr_say++;
                    bulundu=1;
                    not_top+=gecici->not_ort;
                    if(gecici->not_ort<60)
                        altmis_alti_say++;
                }
                gecici=gecici->sonraki;
            }
        }
    }
    if(bulundu==1)
    {
        printf("\nSinifin ogrenci sayisi:%d\n",ogr_say);
        printf("Sinifin genel not ortalamasi:%.2f\n",not_top/ogr_say);
        printf("Donemlik agirlikli not ortalamasi 60 in altinda olanlar ogrenci sayisi %d\nAltmis alti yuzdesi:%.2f\n",altmis_alti_say,(float)(altmis_alti_say*100)/ogr_say);
    }
    else
        printf("Bu sinifta ogrenci bulunmamaktadir!!!\n");
}

void kucukten_buyuge_listele(struct dersin_ogrencisi *liste_basi,struct ogrenciler *yapi_dizisi[])
{
    struct dersin_ogrencisi *gecici;
    struct ogrenciler *bir_ogrenci=malloc(sizeof(struct ogrenciler));
    int hash,ogr_no;

    printf("Dersi alan ogrenciler:\n");
    printf("Ogr No    Ad Soyad        Sinif     Not Ort\n");
    printf("---------  -------------  ----       ------\n");

    gecici=(liste_basi)->noya_gore_sonraki;
    while (gecici!=liste_basi)
    {
        ogr_no=gecici->ogr_no;
        hash=(ogr_no-1)/100;
        bir_ogrenci=noya_gore_ogr_ara(&yapi_dizisi[hash],ogr_no);

        if(bir_ogrenci!=NULL)
        {
            printf("%-9d  %-7s  %-5d    %.2f\n",bir_ogrenci->ogr_no,bir_ogrenci->ad_soyad,bir_ogrenci->sinif,bir_ogrenci->not_ort);
        }

        gecici=gecici->noya_gore_sonraki;
    }
}



