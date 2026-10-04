# Cisco Networking Labs

This repository contains three practical networking labs built using **Cisco Packet Tracer**.

The projects demonstrate hands-on experience with IP addressing, subnetting, VLSM, DHCP, static IP configuration, Cisco router and switch configuration, and network troubleshooting.

## 1. Two Branches — One DHCP Server

**Objective:** Configure communication between two different networks using a single router.

**Technologies:**
- Cisco Router and Switches
- DHCP Server
- Static IP Addressing
- Inter-network Communication
- ICMP (Ping)

**Network Configuration:**

| Branch | Network | Gateway | Configuration |
|---|---|---|---|
| Branch 1 | 192.168.50.0/24 | 192.168.50.1 | DHCP |
| Branch 2 | 10.50.20.0/24 | 10.50.20.1 | Static |

**Testing:** Verify communication between PC1 and PC4 using ping.

## 2. VLSM — Three Branches

**Objective:** Divide a network into three subnets using Variable Length Subnet Masking (VLSM).

**Main Network:** `172.20.0.0/24`

| Branch | Required Hosts | Subnet | Gateway |
|---|---|---|---|
| Branch 1 | 100 | 172.20.0.0/25 | 172.20.0.1 |
| Branch 2 | 50 | 172.20.0.128/26 | 172.20.0.129 |
| Branch 3 | 25 | 172.20.0.192/27 | 172.20.0.193 |

**Skills Practiced:**
- VLSM calculations
- Subnet allocation
- Router interface configuration
- Static IP addressing
- Connectivity troubleshooting

**Testing:** Verify communication between computers belonging to different subnets.

## 3. Startup Network — VLSM, DHCP and Static IP

**Objective:** Design a network infrastructure for a startup with four departments.

**Main Network:** `192.168.50.0/24`

| Department | Hosts | Subnet | Gateway | Addressing |
|---|---|---|---|---|
| Developers | 60 | 192.168.50.0/26 | 192.168.50.1 | DHCP |
| Designers | 25 | 192.168.50.64/27 | 192.168.50.65 | Static |
| Marketing | 12 | 192.168.50.96/28 | 192.168.50.97 | DHCP |
| Management | 5 | 192.168.50.112/29 | 192.168.50.113 | Static |

**Network Infrastructure:**
- 1 Cisco Router
- 4 Cisco Switches
- 2 DHCP Servers
- 8 PCs

**Security Configuration:**
- Device hostnames
- Console passwords
- Enable secret passwords
- MOTD warning banners

**Testing:** Verify communication between Developers and Management using ICMP ping.

## Tools and Technologies

- Cisco Packet Tracer
- Cisco IOS CLI
- IPv4
- VLSM and Subnetting
- DHCP
- Static IP Addressing
- ICMP
- Basic Network Security

## Learning Outcomes

These labs helped me practice designing, configuring, and troubleshooting small business networks.

They also strengthened my understanding of networking fundamentals relevant to DevOps and cybersecurity.

## Author

**Vahe Hovakimyan**

GitHub: https://github.com/vahehovak15-wq