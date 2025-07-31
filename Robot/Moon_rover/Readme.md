# 3D Model of a Robot for Path Following  

### Folder Structure  

- **DXF** – Cutting files with a list of required parts  
- **photos** – Photos of the 3D model in SolidWorks  
- **PDF_schemes** – PDF drawings  
- **SolidWorksModel** – 3D model files for SolidWorks  

### **Component List**

#### **1. Laser-Cut Parts (Acrylic)**
| Name | Quantity | Material / Note |
|---|---|---|
| antenna | 1 | White acrylic |
| antenna_circle | 1 | White acrylic |
| battery_divider | 1 | White acrylic |
| floor_1 | 1 | White acrylic |
| floor_2 | 1 | White acrylic |
| floor_3 | 1 | White acrylic |
| front_panel | 1 | White acrylic |
| rear_panel | 1 | White acrylic |
| side_panel | 2 | White acrylic |
| windshield | 1 | Clear acrylic (options: transparent orange/blue) |

**Total:** 11 parts

---

#### **2. Sensors and Electronics**
- **Control Board** — 1 pc.
- **Line Follower Sensor** — Octoliner v2 ([link](https://amperka.ru/product/zelo-follow-line-sensor))
- **Color Sensor** — TCS34725 ([link](https://shop.robotclass.ru/item/2310))
- **Motor** — JGB37-520 2 pcs. (178 RPM, with encoders) ([link](https://shop.robotclass.ru/item/2080))
- **LED** — 8 mm, white ([link](https://shop.robotclass.ru/item/2103))

---

#### **3. Cables and Connectors (JST)**
- Double-sided JST PH2.0 (6 pin, 10 cm) — 2 pcs. (motors)
- Single-sided JST PH2.0 (6 pin, 10 cm) — 1 pc. (color sensor)
- Single-sided JST PH2.0 (4 pin, 10 cm) — 1 pc. (line follower sensor)

---

#### **4. Other Components**
- **Battery** — 6S2P
- **JGB37 Motor Mount** — 2 pcs. ([link](https://shop.robotclass.ru/item/313))
- **Brass Coupler** (inner diameter 6 mm) — 2 pcs. ([link](https://shop.robotclass.ru/item/806))
- **Wheels** (65–68 mm) — 2 pcs. ([link](https://shop.robotclass.ru/item/1172))
- **XT-60 Connector (female)** — 1 pc.
  ![image](https://camo.githubusercontent.com/80d5e606f7099f9aa936a25d4df52132eb1070366be52e3b65bb9cadc1558092/68747470733a2f2f63646e2e786d692e65652f313932382d73757065726c617267655f64656661756c742f78742d36302d72617a656d2d6d616c652d66656d616c652d3630612e6a7067)
- **Wires for Charging Module** — 2 pcs. (red and black)
- **Ring Terminal (M3)** — 2 pcs.
- **Textolite** — 2 pcs. (45×70 mm)

---

#### **5. Fasteners**

##### **Ball Caster**
- M3×8 mm screw — 2 pcs.
- M3 nut (locking) — 2 pcs.
- M3 washer — 2 pcs.
##### **Color Sensor**
- M2×8+ mm screw — 2 pcs.
- M2 nut — 2 pcs.
##### **Line Follower Sensor**
- M3×8–10 mm screw — 4 pcs.
- M3 nut (locking) — 2 pcs.
- M3 nut — 2 pcs.
##### **Control Board**
- M3×14 mm screw — 4 pcs.
- M3 brass standoff (5–6 mm) — 4 pcs.
- M3 nut — 4 pcs.
##### **Floor Mounts**
- M3 brass standoff (45 mm) — 4 pcs.
- M3×6+ mm screw — 4 pcs.
- M3 nut — 4 pcs.
##### **Roof Mounts**
- M3 brass standoff (50 mm) — 4 pcs.
- M3×6+ mm screw — 4 pcs.
- M3 nut — 4 pcs.
##### **Motors**
- M3×10 mm screw — 8 pcs.
- M3 nut (locking) — 8 pcs.
- M3×6 mm screw — 8–12 pcs.
##### **Wheels**
- M4 screw — 2 pcs.
##### **Antenna**
- M3×12–14 mm screw — 1 pc.
- M3 nut (locking) — 1 pc.
##### **Charging Module**
- M3×8 mm screw — 8 pcs.
- M3 nut — 8 pcs.

#### **Comprehensive Fastener List**

| **Fastener Type** | **Quantity** | **Note** |
|---|---|---|
| **Screws** | | |
| M2×8+ mm screw | 2 pcs. | |
| M3×6 mm screw | 8–12 pcs. | |
| M3×6+ mm screw | 8 pcs. | |
| M3×8 mm screw | 10 pcs. | (2 – ball caster, 8 – charging module) |
| M3×10 mm screw | 8 pcs. | |
| M3×12–14 mm screw | 5 pcs. | (1 – antenna, 4 – control board) |
| M4 screw | 2 pcs. | |
| **Nuts** | | |
| M2 nut | 2 pcs. | |
| M3 nut | 31 pcs. | (standard) |
| M3 nut (locking) | 13 pcs. | |
| **Washers** | | |
| M3 washer | 2 pcs. | |
| **Standoffs** | | |
| M3 brass standoff (5–6 mm) | 4 pcs. | |
| M3 brass standoff (45 mm) | 4 pcs. | |
| M3 brass standoff (50 mm) | 4 pcs. | |

---

### **Assembly Instructions**
1. Solder wires to the sensors and LED; solder wires for the charging module.
2. Screw the line follower sensor, color sensor, and ball caster to the first floor.
3. Secure the motors to the mounts.
4. Screw the motor mounts to the second floor.
5. Secure the control board to the second floor.
6. Use the 45 mm standoffs to screw the first floor to the second floor.
7. Place the battery.
8. Screw the antenna to the side panel.
9. Thread the wires and connect them to the control board.
10. Upload the program to the robot, if not already done.
11. Glue the LED to the front panel and connect the LED.
12. Screw the 50 mm standoffs for the roof mount to the second floor.
13. Screw the textolite and power terminals to the rear panel.
14. Connect the battery and check that everything is working.
15. Install all side panels and the windshield.
16. Install and screw down the roof.
