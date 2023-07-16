# HTTP REST Client Get and Parse JSON example

This is an example of getting json from the internet and parsing it.

In this example, we create a wifi connection using your ssid and password (edit this in the main.c file)
and then get an https endpoint every second.

# Configure the Project

- open the project config menu (`idf.py menuconfig`)
- Configure the buffers under "HTTP REST Client"

# Build and Flash

```
idf.py -p PORT flash monitor
```

# Example Log Output

```
I (0) cpu_start: App cpu up.
I (492) cpu_start: Pro cpu start user code
I (492) cpu_start: cpu freq: 160000000 Hz
I (492) cpu_start: Application information:
I (497) cpu_start: Project name:     http-client-test
I (503) cpu_start: App version:      1
I (507) cpu_start: Compile time:     Jul 16 2023 17:56:37
I (513) cpu_start: ELF file SHA256:  6b189db1b463ed7d...
I (519) cpu_start: ESP-IDF:          v5.0.2-dirty
I (525) cpu_start: Min chip rev:     v0.0
I (529) cpu_start: Max chip rev:     v3.99
I (534) cpu_start: Chip rev:         v3.0
I (539) heap_init: Initializing. RAM available for dynamic allocation:
I (546) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (552) heap_init: At 3FFB7CA0 len 00028360 (160 KiB): DRAM
I (559) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (565) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (571) heap_init: At 40094D4C len 0000B2B4 (44 KiB): IRAM
I (579) spi_flash: detected chip: generic
I (582) spi_flash: flash io: dio
I (588) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
I (597) main: Starting app_main...
I (667) wifi:wifi driver task: 3ffbfd8c, prio:23, stack:6656, core=0
I (667) system_api: Base MAC address is not set
I (667) system_api: read default base MAC address from EFUSE
I (697) wifi:wifi firmware version: 57982fe
I (697) wifi:wifi certification version: v7.0
I (697) wifi:config NVS flash: enabled
I (697) wifi:config nano formating: disabled
I (697) wifi:Init data frame dynamic rx buffer num: 32
I (707) wifi:Init management frame dynamic rx buffer num: 32
I (707) wifi:Init management short buffer num: 32
I (717) wifi:Init dynamic tx buffer num: 32
I (717) wifi:Init static rx buffer size: 1600
I (727) wifi:Init static rx buffer num: 10
I (727) wifi:Init dynamic rx buffer num: 32
I (737) wifi_init: rx ba win: 6
I (737) wifi_init: tcpip mbox: 32
I (737) wifi_init: udp mbox: 6
I (747) wifi_init: tcp mbox: 6
I (747) wifi_init: tcp tx win: 5744
I (747) wifi_init: tcp rx win: 5744
I (757) wifi_init: tcp mss: 1440
I (757) wifi_init: WiFi IRAM OP enabled
I (767) wifi_init: WiFi RX IRAM OP enabled
I (847) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (947) wifi:mode : sta (b8:d6:1a:01:e7:a8)
I (947) wifi:enable tsf
I (957) main: Waiting for WiFi connection...
I (957) WIFI_EVENT: WiFi station connecting...
I (967) wifi:new:<4,0>, old:<1,0>, ap:<255,255>, sta:<4,0>, prof:1
I (2047) wifi:state: init -> auth (b0)
I (2067) wifi:state: auth -> assoc (0)
I (2067) wifi:state: assoc -> run (10)
I (2177) wifi:connected with Rising Orchards Guest, aid = 18, channel 4, BW20, bssid = b2:9e:43:69:d3:83
I (2177) wifi:security: WPA2-PSK, phy: bgn, rssi: -43
I (2187) wifi:pm start, type: 1

I (2227) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (3247) wifi:<ba-add>idx:0 (ifx:0, b2:9e:43:69:d3:83), tid:0, ssn:0, winSize:64
I (4187) esp_netif_handlers: sta ip: 192.168.1.43, mask: 255.255.255.0, gw: 192.168.1.1
I (4187) main: WiFi connected
I (4187) main: Starting Main Loop...
I (4187) main: Fetching Data from URL: https://jsonplaceholder.typicode.com/todos/1
I (5617) esp-x509-crt-bundle: Certificate validated
I (6517) main: Raw Response string:
{
  "userId": 1,
  "id": 1,
  "title": "delectus aut autem",
  "completed": false
}
I (6517) main: Parsed correctly!
I (6527) main: Response JSON:
{
        "userId":       1,
        "id":   1,
        "title":        "delectus aut autem",
        "completed":    false
}
I (6537) main: Looping in 1 sec...
```
