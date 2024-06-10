#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

const int MAX_PRODUK = 100;  

struct Produk {
    string code;
    string nama;
    int harga;
    int jumlah;
};

string getWaktuSaatIni() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
    return string(buffer);
}

void printProduk(const Produk produk[], int size) {
    cout << "-------------------------------------------------" << endl;
    cout << "| Kode | Nama                 | Harga           |" << endl;;
    cout << "-------------------------------------------------" << endl;;
    for (int i = 0; i < size; i++) {
        cout << "| " << setw(4) << produk[i].code 
             << " | " << setw(20) << produk[i].nama 
             << " | " << setw(15) << produk[i].harga << " |" << endl;
    }
    cout << "-------------------------------------------------" << endl;
}

Produk cariProduk(const Produk produk[], int size, const string& identifier) {
    for (int i = 0; i < size; i++) {
        if (produk[i].code == identifier || produk[i].nama == identifier) {
            return produk[i];
        }
    }
    throw invalid_argument("Produk tidak ditemukan");
}

class Billing {
private:
    bool isPaid;
    double banyakBayar;
    double kembalian;

public:
    vector<Produk> keranjang;
    Billing() : isPaid(false), banyakBayar(0), kembalian(0) {}

    void tambahProduk(const Produk& p) {
        if (keranjang.size() >= MAX_PRODUK) {
            throw runtime_error("Jumlah maksimum produk tercapai!");
        }
        keranjang.push_back(p);
    }

    void tampilkanBill() {
        int totalHarga = 0;
        cout << "========================= Supermarket Billing =========================" << endl;
        cout << "--------------------------- Bill Receipt ------------------------------" << endl;
        cout << "Tanggal dan Waktu: " << getWaktuSaatIni() << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        cout << "| No. | Nama Produk        | Harga (Ribu)    | Jumlah | Total (Ribu)  |" << endl;
        cout << "-----------------------------------------------------------------------" << endl;
        for (size_t i = 0; i < keranjang.size(); i++) {
            const Produk& p = keranjang[i];
            int total = p.harga * p.jumlah;
            totalHarga += total;
            cout << left 
                 << "|" << setw(5) << (i + 1)
                 << "| " << setw(19) << p.nama 
                 << "| " << setw(16) << p.harga 
                 << "| " << setw(7) << p.jumlah 
                 << "| " << setw(14) << total 
                 << "|" << endl;
        }
        cout << "-----------------------------------------------------------------------" << endl;
        cout << "Total Harga: Rp " << totalHarga << endl;
    }

    double getBanyakBayar() const {
        return banyakBayar;
    }

    double getKembalian() const {
        return kembalian;
    }

    bool sudahDibayar() const {
        return isPaid;
    }

    void pembayaran() {
        tampilkanBill();

        cout << "Masukkan jumlah yang dibayarkan oleh pelanggan: ";
        cin >> banyakBayar;

        double total = hitungTotal();
        kembalian = banyakBayar - total;

        cout << "-----------------------------------------------------------------------" << endl;
        if (banyakBayar >= total) {
            cout << "Pembayaran berhasil. " << endl;
            cout << "Kembalian: Rp " << kembalian << endl;
            isPaid = true;
        } else {
            cout << "Pembayaran gagal. Jumlah yang dibayarkan tidak cukup." << endl;
            banyakBayar = 0;
            kembalian = 0;
        }
    }

    int hitungTotal() const {
        int totalHarga = 0;
        for (const auto& p : keranjang) {
            totalHarga += p.harga * p.jumlah;
        }
        return totalHarga;
    }
};

void tambahProduk(Billing& billing, Produk produk[], int size) {
    string idNama; 
    int jumlah; 
    char tambahLagi; 

    do { 
        cout << "Masukkan kode atau nama produk yang dibeli: ";
       getline(cin, idNama);
        cout << "Masukkan jumlah produk: ";
        cin >> jumlah;

        try {
            Produk produkTerpilih = cariProduk(produk, size, idNama);
            produkTerpilih.jumlah = jumlah;
            billing.tambahProduk(produkTerpilih);
            cout << "Produk berhasil ditambahkan ke keranjang!" << endl;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            break;  
        }

        cout << "Apakah Anda ingin menambah produk lagi? (y/n): ";
        cin >> tambahLagi;
    } while ((tambahLagi == 'y' || tambahLagi == 'Y') && billing.keranjang.size() < MAX_PRODUK);
}

void kembali() {
    cout << "Tekan Enter untuk kembali ke menu utama: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get(); 
}

int main() {
    Produk produk[] = {
        {"P001", "Apel", 15000, 0},
        {"P002", "Jeruk", 7500, 0},
        {"P003", "Bayam", 12500, 0},
        {"P004", "Kangkung", 15000, 0},
        {"P005", "Daging Sapi Giling", 75000, 0},
        {"P006", "Daging Ayam", 35000, 0},
        {"P007", "Ikan Salmon", 85000, 0},
        {"P008", "Ikan Tongkol", 29000, 0},
        {"P009", "Susu Sapi Segar", 12500, 0},
        {"P010", "Susu Kambing Segar", 15000, 0},
        {"P011", "Beras", 160000, 0},
        {"P012", "Pasta", 4500, 0},
        {"P013", "Mie Instan", 3500, 0},
        {"P014", "Minyak", 22000, 0},
        {"P015", "Tepung", 12000, 0},
        {"P016", "Keripik Kentang", 8000, 0},
        {"P017", "Biskuit Coklat", 15000, 0},
        {"P018", "Permen", 750, 0},
        {"P019", "Coklat", 12500, 0},
        {"P020", "Tomat Kaleng", 15000, 0},
        {"P021", "Daging Kaleng", 50000, 0},
        {"P022", "Soda", 7500, 0},
        {"P023", "Teh", 1500, 0},
        {"P024", "Kopi", 1000, 0},
        {"P025", "Wajan", 125000, 0},
        {"P026", "Blender", 150000, 0},
        {"P027", "Panci", 250000, 0},
        {"P028", "Gelas", 5000, 0},
        {"P029", "Piring", 8000, 0},
        {"P030", "Garpu", 1250, 0},
        {"P031", "Sendok", 1250, 0},
        {"P032", "Deterjen", 1500, 0},
        {"P033", "Pemutih", 750, 0},
        {"P034", "Pengharum Ruangan", 15000, 0},
        {"P035", "Sabun", 3500, 0},
        {"P036", "Sampo", 12500, 0},
        {"P037", "Sikat Gigi", 15000, 0},
        {"P038", "Pasta Gigi", 75000, 0},
        {"P039", "Handuk", 12500, 0},
        {"P040", "Lampu", 15000, 0},
        {"P041", "Baterai", 16000, 0},
        {"P042", "Kabel Isi", 30000, 0},
        {"P043", "Kaos", 10000, 0},
        {"P044", "Pakaian Dalam", 10000, 0},
        {"P045", "Kaus Kaki", 5000, 0},
        {"P046", "Deodoran", 6000, 0},
        {"P047", "Lemari Es", 3000000, 0},
        {"P048", "AC", 2500000, 0},
        {"P049", "Oven", 1250000, 0},
        {"P050", "Kompor", 150000, 0},
        {"P051", "Televisi", 1670000, 0},
        {"P052", "Mainan Anak", 50000, 0},
        {"P053", "Susu Bayi", 70000, 0},
        {"P054", "Makanan Bayi", 30000, 0},
        {"P055", "Pakaian Anak", 40000, 0},
        {"P056", "Pena", 1500, 0},
        {"P057", "Pensil", 1000, 0},
        {"P058", "Buku", 2500, 0},
        {"P059", "Kertas", 100, 0},
        {"P060", "Penghapus", 2000, 0},
        {"P061", "Vitamin", 5000, 0},
        {"P062", "Perban", 8000, 0},
        {"P063", "Obat", 10000, 0},
        {"P064", "Makanan Kucing", 25000, 0},
        {"P065", "Makanan Anjing", 30000, 0},
        {"P066", "Makanan Kecoa", 10000, 0},
        {"P067", "Bir", 20000, 0},
        {"P068", "Rokok", 25000, 0},
        {"P069", "Suplemen", 45000, 0},
        {"P070", "Obat Herbal", 35000, 0},
    };
    int size = sizeof(produk) / sizeof(produk[0]);
    
    Billing billing;
    int pilihan;

    do {
        system("CLS");
        cout << "===== Supermarket Billing =====" << endl;
        cout << "1. Daftar Produk" << endl;
        cout << "2. Tambah Produk" << endl;
        cout << "3. Tampilkan Bill" << endl;
        cout << "4. Pembayaran" << endl;
        cout << "5. Keluar" << endl;
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                printProduk(produk, size);
                kembali();
                break;
            case 2:
                tambahProduk(billing, produk, size);
                kembali();
                break;
            case 3:
                system("CLS");
                billing.tampilkanBill();
                kembali();
                break;
            case 4:
                system("CLS");
                billing.pembayaran();
                kembali();
                break;
            case 5:
                system("CLS");
                billing.tampilkanBill();
                if (billing.sudahDibayar()) {
                    cout << "Jumlah Bayar: Rp " << billing.getBanyakBayar() << endl;
                    cout << "-----------------------------------------------------------------------" << endl;
                    cout << "Kembalian: Rp " << billing.getKembalian() << endl;
                }
                cout << "Terima kasih telah menggunakan Supermarket Billing!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                kembali();
        }
    } while (pilihan != 5);

    system("Pause");
    return 0;
}