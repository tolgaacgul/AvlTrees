#include<stdio.h>
#include<stdlib.h>

// AVL ağaç düğümü için tanımlamaları yapıyoruz
struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};

// İki tam sayıdan maximumu almak için kullandığımız hazır fonk
int max(int a, int b);

// Ağacın yüksekliğini bulmak için
int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height; //aksi halde yüksekliği döndürür
}

//iki tamsayının maximumunu bulan fonk
int max(int a, int b)
{
    return (a > b)? a : b;

}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key)
{
    struct Node* node = (struct Node*)
            malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially added at leaf
    return(node);
}

//alt ağacı sağa döndüren yardımcı fonk.
struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    //Döndürme yapmak için
    x->right = y;
    y->left = T2;

    // Yüksekliği yeniden hesaplamak için
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // yeni kökü return ediyoruz
    return x;
}

// sola döndürmek için kullanılan yardımcı fonk.
struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    //döndürme yapar
    y->left = x;
    x->right = T2;

    //yükseklikleri yeniden hesaplıyoruz
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    //yeni kök
    return y;
}

//N düğümümünün denge faktörü
//bu denge faktörüne göre döndürme ilemleri yaparız
int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* node, int key)
{
    //1.BST kurallarına göre ekleme işlemi yaparız
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // eşitik durumunda herhangi bir ekleme yok,eklem yapılmıyor yani
        return node;

    //2. atadüğümün yüksekliğini güncelemek için
    node->height = 1 + max(height(node->left),height(node->right));

    /* 3. Ata düğümün dengesiz olup olmadığını
     *  kontrol etmek için denge faktörünü alırız */
    int balance = getBalance(node);

    // 4 tane dengesizlik durumu var bunların arasından hangisi olduğunun kontrolü için

    // sol-sol durumu
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // sağ-sağ durumu
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // sol-sağ durumu
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // sağ-sol durumu
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* değişmemiş düğüm işaretini döndürür */
    return node;
}

// Boş olmayan bir ikili arama ağacı verildiğinde,bu ağaçta bulunan minimum anahtar değeri ile düğümü döndürür.
//tüm ağacın aranmış olmasına gerek yok
// Minumum değer aranır
struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* en soldaki yağrağı bulmak için aşağı doğru döngü*/
    while (current->left != NULL)
        current = current->left;

    return current;
}

//verilen kök ile alt ağaçtan verilen bir anahtar ile bir düğümü silmek için recurcive
//fonksiyonu kullanacağız
//değiştirilen alt ağacın kökü dödürülür

struct Node* deleteNode(struct Node* root, int key)
{
    //standart BST silme işlemi

    if (root == NULL)
        return root;

    // Silinecek anahtar kökün anahtarından küçükse sol alt ağaçta yer alır.
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    // Silinecek anahtar kökün anahtarından büyükse, o zaman sağ alt ağacın içinde bulunur.
    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    // eğer anahtar kökün anahtarıyla aynı ise, o zaman Silinecek düğüm budur.
    else
    {
        // sadece bir çocuğu olan veya hiç çocuğu olmayan düğüm
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                    root->right;

            // çocuğu olmayan durum
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // bir çocuk
                *root = *temp; //boş olmayan çocuğun içeriğini kopyalar
            free(temp);
        }
        else
        {
            // İki çocuklu düğüm: Ardışık miras al (sağ alt ağaçta en küçük)
            struct Node* temp = minValueNode(root->right);

            // Inorder verilerini bu düğüme kopyalanır
            root->key = temp->key;

            //silinir varis
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // ağacın sadce bir düğümü varsa başa geri dön
    if (root == NULL)
        return root;

    //mevcut düğümün yüksekliği güncellenir.
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // bu düğümün denge faktörü alınır (bu düğümün dengesiz olup olmadığını kontrol etmek için)
    int balance = getBalance(root);

    // bu düğüm dengesiz hale gelirse yine bilinen dör durum oluşur

    // sol-sol durumu
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // sol-sağ durumu
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // sağ-sağ durumu
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // sağ-sol durumu
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// ağcımızı preorder olarak çıktı vermesini sağlar.
void preOrder(struct Node *root)
{
    if(root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}




int main()
{
    struct Node *root = NULL;
    int adet,kokGir,DevamMi=1,secenek=0,silinecekSayi,ekSecenek;


    while(DevamMi==1){
        printf("\n\nLütfen İşlem Seçiniz: \n");
        printf("[1] Ağaç Oluştur\n");
        scanf("%d",&secenek);

        if(secenek==1){
            printf("Lütfen Eklenecek Düğüm Sayısını Giriniz: ");
            scanf("%d",&adet);
            for(int i=1;i<adet+1;i++){
                printf("Kök değerini giriniz:");
                scanf("%d",&kokGir);
                root = insert(root,kokGir);
            }
            printf("Oluşturduğunuz ağaçta dengesizlikler döndürme işlemi ile düzeltilir: \nPreOrder Dizilimimiz:\n");
            preOrder(root);

            printf("\n\nOluşturduğunuz ağaçta ek işlem yapmak için Birini seçiniz: \n[1] Ağaçtan Düğüm Sil\n[2] Ağaca Düğüm Ekle\n[3] Çıkış Yap\n");
            scanf("%d",&ekSecenek);
            while(ekSecenek==1||ekSecenek==2){
                if(ekSecenek==1){
                    printf("Lütfen Silmek İstediğiniz Düğüm Numarasını Giriniz: ");
                    scanf("%d",&silinecekSayi);

                    root = deleteNode(root, silinecekSayi);
                    printf("\nSilmek istediğiniz sayıdan sonraki preorder ifade \n");
                    preOrder(root);
                }else if(ekSecenek==2){
                    printf("Kök değerini giriniz:");
                    scanf("%d",&kokGir);
                    root = insert(root,kokGir);
                    printf("Ekleme sonrasındaki AVL ağacımızın preorder dizilimi;\n");
                    preOrder(root);
                }
                printf("\n\nOluşturduğunuz ağaçta ek işlem yapmak için Birini seçiniz: \n[1] Ağaçtan Düğüm Sil\n[2] Ağaca Düğüm Ekle\n[3] Çıkış Yap\n");
                scanf("%d",&ekSecenek);
            }

        }

        //Yeni Bir döngü oluşturmak için
        printf("\n\nYeni ağaç için         [1]'e \n"
               "Tamamen Çıkmak için    [0]'a basınız");
        scanf("%d",&DevamMi);

        root=NULL;

    }//while döngü sonu


    return 0;
}
