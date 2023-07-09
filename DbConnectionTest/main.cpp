#include <mysql_driver.h>
#include <mysql_connection.h>


int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    // Bağlantı oluşturma
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "kullanici_adi", "sifre");

    //elimdeki CSV'ye örnek
    std::string data = "itemid,lastvalue\n21,34\n2231,3223131\n56,45";

    // CSV string
    std::string keys = data.substr(0, data.find_first_of('\n')); //npos dönme ihtimaline karşı dikkatli ol.
    std::string csvString = data.substr(data.find_first_of('\n'), data.size());

    // INSERT sorgusu
    sql::Statement *stmt;
    stmt = con->createStatement();
    stmt->execute("INSERT INTO tablo_adi (sütun1, sütun2, sütun3) VALUES " + csvString);
    delete stmt;

    delete con;
}