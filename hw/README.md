# hardware

## Ripple Tank PCB Build Document
### PCB Render
<img width="1116" height="926" alt="image" src="https://github.com/user-attachments/assets/bbc12293-0b87-4599-856c-aa604db60807" />

## BOM / Parts to populate (not including pre-fabbed items)

| ID               | Component                       | Supplier Code | Supplier LInk                                               |
|---               |---                              |---            |---                                                          |
| F1               | 3A Littlefuse                   | 541-3647P     | https://uk.rs-online.com/web/p/non-resettable-fuses/5413647 |
| J14 (Tank Level) | 3 Pin Terminal Block 5.08mm     | 193-0592      | https://uk.rs-online.com/web/p/pcb-terminal-blocks/1930592  |
| A1               | Arduino IoT 33 with header pins |               |  (in stock)                                                 |
| A1               | 15 pin header sockets x2        |          | https://www.amazon.co.uk/EGSCST-Connector-Breakaway-Connectors-Electronic/dp/B0FFT6C3KP |
| J7, J8, J9       | 2 Pin Terminal Block 5.08mm     |               |  https://uk.rs-online.com/web/p/pcb-terminal-blocks/1930564    | 
| J10              | 4 Pin Terminal Block 5.08mm     |   268-4632    | https://uk.rs-online.com/web/p/pcb-terminal-blocks/2684632      | 
| A2               | DRV8825 Stepper Module with header pins |       |  in stock     |  
| A2               | 8 pin header sockets x2         |               | https://www.amazon.co.uk/EGSCST-Connector-Breakaway-Connectors-Electronic/dp/B0FFT6C3KP        |
| PT1              | 10k multi-turn trim pot         |               |    https://uk.rs-online.com/web/p/trimmer-potentiometers/7885088      |



## Jumper Settings

| ID  | Action  | Condition | Purpose |
|---  |---       |---      |---        |
| JP3 | Short 2 & 3 | Always | Set I2c Addr for BME280 |
| JP6 | Short       | IF LED does not achieve nessissary brightness | set LED Imax to 660mA   |
| JP7 | Short        | IF LED still does not acheive nessissary brightness | set LED Imax to 990mA  |
| JP9 | Short 2 & 3  | Always |  enable audio input CH1 to class D amp IC |
| JP1 | Short 1 & 2  | Always | ground CH2 of Class D amp IC | 
| JP13 | Short       | Always  |   Set voltage to MCU to 3v3  |


All other jumpers remain in initial state
