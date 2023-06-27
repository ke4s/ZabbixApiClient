
# Zabbix API Client

Zabbix api ile iletişime geçmenizi sağlayacak client programı.

İstediğiniz isteği flagler ile ya da json dosyası ile atmanızı sağlayacak ve gelen response'u olduğu gibi ya da CSV formatında çıktı verir.

### FLAGLER

    --help                produce help message
    --url arg             The Zabbix API's url
    --apipath arg         The Zabbix API's path def = "/zabbix/ api_jsonrpc.php"
    --username arg        The Zabbix API's Username
    --password arg        The Zabbix API's Password
    --method arg          The method, that you want to use.
    --httpmethod arg      HTTP method
    --param arg           (OPT) Parameters, that you want to use parameters for request.
                            Usage: param:value
                        
    --output arg          (OPT) Keys, that you want to recieve their datas.
                            Usage: key
                        
    --filter arg          (OPT) Filter the data.
                            Usage: --filter key:data
                        
    --outputformat arg    Output format. You can use CSV JSON. Default JSON
    --keytoprint arg      Keys for CSV format.
    
    --file arg            (OPT) File to read arguments
    --jsonfile arg        (OPT) A JSON file for request's params.





## Kullanım ve örnekleri

Programa vermeniz gereken bilgiler şunlardır.

--url --username --password --method --httpmethod

bu bilgiler ile api'ye bağlanır ve isteği atar. İsteğin elimizde sadece methodu olduğu için beklediğinizden uzak bir çıktı ile karşılaşabiliriz.

Burada isteğin "params" kısmını doldurmamız gerek. Bunu yapma yolunu size bir örnekle açıklayayım.

İsteğimizin methodu "item.get" olsun.

Bu da isteğimizin "params" bölümü olsun.
Bir dosyada olduğunu varsayalım ve dosya ismi de "ornek.json" olsun.

Dosya:ornek.json
```json
{
    "selectChildren": "extend",
    "selectParents": "extend",
    "filter": {
        "hostid": 10583
    },
    "output": [
        "extend",
        "itemid",
        "lastvalue"
    ]
}
```

yukarıdaki json dosyasını aşağıdaki gibi direkt olarak programa verip istediğimiz çıktıyı alabiliriz.


```bash
./zabbixClient --url zabbix.com --username admin --password pass --httpmethod GET --method item.get --jsonfile ornek.json
```

bu şekilde çalıştırdığımızda istek atılacak ve çıktısı json formatında ekrana basılacaktır.



alternatif yol olarak (--jsonfile kullanmadan) şu şekilde çalıştırabiliriz.

Programda, isteğin "params" bölümünü doldurmamızı sağlayan flagler mevcut.

--params --output --filter

bu flagler ile programı çalıştıralım.
```bash
./zabbixClient --url zabbix.com --username admin --password pass --httpmethod GET --method item.get 
--params selectChildren:extend --params selectParents:extend --filter hostid:10583 --output extend 
--output itemid  --output lastvalue
```

gördüğünüz gibi argümanları ver Allahım ver.

Bu kabalalıktan kurtulmak için --file flag i ile argümanları çekebileceği bir dosya verebiliyoruz. Örnek olarak çok fazla değiştirmemizin gerekmediği api bilgilerini dosyadan çekmesini inceleyelim.


Dosya:config
```txt
url = zabbix.com
username = admin
password = pass
```

Dosyamıza kolay kolay değişmeyecek bilgileri verdik.
Şimdi bu dosyayı kullanarak programı çalıştıralım.

```bash
./zabbixClient --file config --httpmethod GET --method item.get --params selectChildren:extend 
--params selectParents:extend --filter hostid:10583 --output extend --output itemid  --output lastvalue
```
Bu çalıştırma şekli ile de tamamen aynı şekilde çalışacaktır.  
* Dosyamıza "--file" hariç tüm flaglerimizi yazabiliriz.



#### CSV çıktısı

Programımızda "--outputformat" isminde bir flag var. bu flag'in default değeri json'dur, bu da response'un ekrana basılmasını sağlar.

Bu flagi kullanarak csv formatında çıktı alacağız. Tabi hangi key'leri çekmek isteğimizi de belirtmemiz lazım. Bunun için de --keytoprint flag'ini kullanacağız.


yine aynı örnekte response'un içinde olan itemid ve lastvalue değerlerini csv formatında ekrana bastıralım.

Dosya:config
```txt
url = zabbix.com
username = admin
password = pass
httpmethod = GET
method = item.get
```

Dosya:ornek.json
```json
{
    "selectChildren": "extend",
    "selectParents": "extend",
    "filter": {
        "hostid": 10583
    },
    "output": [
        "extend",
        "itemid",
        "lastvalue"
    ]
}
```

komut:
```bash
./zabbixClient --file config --jsonfile ornek.json --outputformat csv --keytoprint itemid --keytoprint lastvalue
```

Config dosyasından api bilgileri ve istek tipi ve methodunu çektik. "ornek.json"'dan ise isteğin "params" bölümünü aldık. En sonuna da çıktı tipimizin csv olduğunu ve çıktıda görmek istediklerimizi belirttik.

çıktımız aşağı yukarı şöyle olacaktır.

```txt
itemid,lastvalue
40012,34.4444
40013,21
40014,0
4015,N/A
...
..
.
```

Bence önemli olan bir detay var onu da sizinle paylaşayım.

* Terminalden aldığımız veri her zaman dosyadan aldığımızı ezer.
bu şekilde dosyayı değiştirmeden aynı flag'i terminalde farklı argümanla kullanarak çalıştırabiliriz.

örnek olarak yukarıdaki örnekteki komuta --url olmayanurl.com eklersek eğer dosyada verilmiş olan zabbix.com url'sini es geçerek olmayanurl.com'a istek atmaya çalışacak ve program çalışmayacak. Bu örnekte çalışmasını engelledik ama sizin bu özelliği işe yarar bir şekilde kullanabileceğinize güvenim tam.

teşekkürler.




