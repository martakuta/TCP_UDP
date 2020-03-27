# TCP_UDP

1.uruchom serwer 
    ./echo-server (lub inny serwer)

2.gniazdo jest już w wykazie otwartych gniazd, sprawdź to
    netstat -lunp (UDP) -ltnp (TCP)

3.podłącz się do serwera przez "nc" jako klient, żeby móc wysyłać sobie pakiety i odbierać ewentualne od serwera
    nc -u localhost 10001 (-u dla UDP i -t dla TCP)
    zamiast localhost może być też 0.0.0.0, to to samo
    lub inny adres jeśli serwer jest na innym; 10001 czyli port
    też może być inny, to jest zadeklarowane w serwerze
    jeśli serwer nie jest włączony, to po próbie wysłania
    pierwszej wiadomości połączenie się zerwie

4.w UDP można też podłączyć naraz więcej niż jednego klienta,
    tak samo używając nc

5.w TCP można znaleźć połączenie serwer-klient przez polecenie
    netstat -tnp
