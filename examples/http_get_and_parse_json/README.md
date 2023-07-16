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
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:7076
load:0x40078000,len:15576
load:0x40080400,len:4
0x40080400: _init at ??:?

load:0x40080404,len:3876
entry 0x4008064c
I (29) boot: ESP-IDF v5.1 2nd stage bootloader
I (29) boot: compile time Jul 16 2023 18:11:40
I (29) boot: Multicore bootloader
I (33) boot: chip revision: v3.0
I (37) boot.esp32: SPI Speed      : 40MHz
I (42) boot.esp32: SPI Mode       : DIO
I (46) boot.esp32: SPI Flash Size : 16MB
I (51) boot: Enabling RNG early entropy source...
I (56) boot: Partition Table:
I (60) boot: ## Label            Usage          Type ST Offset   Length
I (67) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (75) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (82) boot:  2 factory          factory app      00 00 00010000 00100000
I (90) boot: End of partition table
I (94) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=391e0h (233952) map
I (187) esp_image: segment 1: paddr=00049208 vaddr=3ffb0000 size=03804h ( 14340) load
I (193) esp_image: segment 2: paddr=0004ca14 vaddr=40080000 size=03604h ( 13828) load
I (199) esp_image: segment 3: paddr=00050020 vaddr=400d0020 size=a0f14h (659220) map
I (439) esp_image: segment 4: paddr=000f0f3c vaddr=40083604 size=12030h ( 73776) load
I (481) boot: Loaded app from partition at offset 0x10000
I (481) boot: Disabling RNG early entropy source...
I (492) cpu_start: Multicore app
I (493) cpu_start: Pro cpu up.
I (493) cpu_start: Starting app cpu, entry point is 0x400813a4
0x400813a4: call_start_cpu1 at C:/Users/parab/esp-v5.1/esp-idf/components/esp_system/port/cpu_start.c:154

I (0) cpu_start: App cpu up.
I (511) cpu_start: Pro cpu start user code
I (511) cpu_start: cpu freq: 160000000 Hz
I (511) cpu_start: Application information:
I (515) cpu_start: Project name:     http-client-test
I (521) cpu_start: App version:      1
I (525) cpu_start: Compile time:     Jul 16 2023 18:11:06
I (532) cpu_start: ELF file SHA256:  1a6f2bf9890316e4...
I (538) cpu_start: ESP-IDF:          v5.1
I (542) cpu_start: Min chip rev:     v0.0
I (547) cpu_start: Max chip rev:     v3.99
I (552) cpu_start: Chip rev:         v3.0
I (557) heap_init: Initializing. RAM available for dynamic allocation:
I (564) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (570) heap_init: At 3FFB8448 len 00027BB8 (158 KiB): DRAM
I (576) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (582) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (589) heap_init: At 40095634 len 0000A9CC (42 KiB): IRAM
I (597) spi_flash: detected chip: generic
I (600) spi_flash: flash io: dio
I (605) app_start: Starting scheduler on CPU0
I (609) app_start: Starting scheduler on CPU1
I (609) main_task: Started on CPU0
I (619) main_task: Calling app_main()
I (619) main: Starting app_main...
I (679) wifi:wifi driver task: 3ffc057c, prio:23, stack:6656, core=0
I (709) wifi:wifi firmware version: b2f1f86
I (709) wifi:wifi certification version: v7.0
I (709) wifi:config NVS flash: enabled
I (709) wifi:config nano formating: disabled
I (709) wifi:Init data frame dynamic rx buffer num: 32
I (719) wifi:Init management frame dynamic rx buffer num: 32
I (719) wifi:Init management short buffer num: 32
I (729) wifi:Init dynamic tx buffer num: 32
I (729) wifi:Init static rx buffer size: 1600
I (739) wifi:Init static rx buffer num: 10
I (739) wifi:Init dynamic rx buffer num: 32
I (749) wifi_init: rx ba win: 6
I (749) wifi_init: tcpip mbox: 32
I (749) wifi_init: udp mbox: 6
I (759) wifi_init: tcp mbox: 6
I (759) wifi_init: tcp tx win: 5744
I (759) wifi_init: tcp rx win: 5744
I (769) wifi_init: tcp mss: 1440
I (769) wifi_init: WiFi IRAM OP enabled
I (779) wifi_init: WiFi RX IRAM OP enabled
I (779) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (889) wifi:mode : sta (b8:d6:1a:01:e7:a8)
I (889) wifi:enable tsf
I (889) main: Waiting for WiFi connection...
I (889) WIFI_EVENT: WiFi station connecting...
I (909) wifi:new:<4,0>, old:<1,0>, ap:<255,255>, sta:<4,0>, prof:1
I (909) wifi:state: init -> auth (b0)
I (919) wifi:state: auth -> assoc (0)
I (929) wifi:state: assoc -> run (10)
I (959) wifi:connected with <redacted>, aid = 18, channel 4, BW20, bssid = 9a:9e:43:b5:87:70
I (959) wifi:security: WPA2-PSK, phy: bgn, rssi: -62
I (969) wifi:pm start, type: 1

I (969) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (979) wifi:<ba-add>idx:0 (ifx:0, 9a:9e:43:b5:87:70), tid:0, ssn:0, winSize:64
I (6469) esp_netif_handlers: sta ip: <redacted>, mask: <redacted>, gw: <redacted>
I (6469) main: WiFi connected
I (6469) main: Starting Main Loop...
I (6469) main: Fetching Data from URL: https://jsonplaceholder.typicode.com/todos/1
I (7749) esp-x509-crt-bundle: Certificate validated
I (8879) main: Raw Response string:
{
  "userId": 1,
  "id": 1,
  "title": "delectus aut autem",
  "completed": false
}
I (8879) main: Parsed correctly!
I (8879) main: Response JSON:
{
        "userId":       1,
        "id":   1,
        "title":        "delectus aut autem",
        "completed":    false
}
I (8889) main: Looping in 1 sec...
```
