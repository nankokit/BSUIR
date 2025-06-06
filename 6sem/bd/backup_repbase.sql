--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-04-06 01:23:39

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 5 (class 2615 OID 25740)
-- Name: main; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA main;


ALTER SCHEMA main OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 224 (class 1259 OID 25782)
-- Name: booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.booking (
    id integer NOT NULL,
    "time" timestamp without time zone,
    cost integer,
    creation_date timestamp without time zone,
    status character varying(255),
    number_of_people integer,
    id_room integer,
    id_user integer,
    duration integer
);


ALTER TABLE main.booking OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 25748)
-- Name: equipment; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.equipment (
    id integer NOT NULL,
    name text,
    type text,
    brand text,
    model text,
    condition text,
    id_rehearsal_point integer
);


ALTER TABLE main.equipment OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 25779)
-- Name: equipment_booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.equipment_booking (
    id_equipment integer,
    id_booking integer
);


ALTER TABLE main.equipment_booking OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 25741)
-- Name: rehearsal_points; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.rehearsal_points (
    id integer NOT NULL,
    rating real,
    contact_number character varying(255),
    schedule json,
    name text,
    address text
);


ALTER TABLE main.rehearsal_points OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 25755)
-- Name: rooms; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.rooms (
    id integer NOT NULL,
    name text,
    air_conditioner boolean,
    price integer,
    recording_support boolean,
    area integer,
    id_rehearsal_point integer
);


ALTER TABLE main.rooms OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 25787)
-- Name: service; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.service (
    id integer NOT NULL,
    name text,
    price integer,
    type text,
    requirements text,
    id_rehearsal_point integer
);


ALTER TABLE main.service OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 25776)
-- Name: service_booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.service_booking (
    id_service integer,
    id_booking integer
);


ALTER TABLE main.service_booking OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 25762)
-- Name: staff; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.staff (
    id integer NOT NULL,
    full_name text,
    address text,
    experience integer,
    phone character varying(255),
    age integer,
    id_rehearsal_point integer
);


ALTER TABLE main.staff OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 25769)
-- Name: users; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.users (
    id integer NOT NULL,
    full_name text,
    phone character varying(255),
    email text,
    registration_date timestamp without time zone
);


ALTER TABLE main.users OWNER TO postgres;

--
-- TOC entry 4939 (class 0 OID 25782)
-- Dependencies: 224
-- Data for Name: booking; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.booking (id, "time", cost, creation_date, status, number_of_people, id_room, id_user, duration) FROM stdin;
39	2025-02-25 06:14:48	35	2025-02-23 13:45:49	completed	6	\N	12	2
29	2023-03-05 10:30:00	30	2023-03-04 08:57:23	canceled	2	\N	2	1
31	2025-03-09 11:24:40	25	2025-03-03 13:52:31	active	3	\N	4	1
30	2025-02-28 12:31:47	70	2025-02-21 18:03:32	completed	6	\N	3	3
32	2025-03-29 00:12:05	45	2025-03-23 00:58:25	active	5	\N	5	4
33	2025-02-27 08:08:26	15	2025-02-25 12:16:31	completed	1	\N	6	2
35	2025-03-10 13:56:02	60	2025-03-04 14:27:17	active	8	\N	8	1
36	2025-02-28 01:53:12	40	2025-02-27 00:02:05	completed	4	\N	9	2
37	2025-03-17 08:23:14	55	2025-03-10 12:33:10	active	3	\N	10	3
40	2025-03-23 21:14:47	65	2025-03-18 13:24:11	active	5	\N	13	4
41	2025-03-22 21:17:41	50	2025-03-21 19:26:59	active	1	\N	14	3
43	2023-03-17 10:00:00	75	2023-03-14 06:06:39	canceled	2	\N	16	1
50	2025-03-09 08:15:57	40	2025-03-08 00:03:31	active	3	\N	23	2
54	2025-02-26 08:48:56	50	2025-02-23 16:30:00	completed	8	\N	27	1
49	2025-03-19 08:40:56	80	2025-03-14 21:30:30	active	5	\N	22	1
46	2025-03-16 12:10:03	65	2025-03-09 18:00:03	active	7	\N	19	2
44	2025-03-18 01:12:44	45	2025-03-11 21:07:32	active	4	\N	17	3
47	2023-03-21 09:45:00	15	2023-03-21 04:09:03	canceled	1	\N	20	4
45	2025-02-21 19:34:14	30	2025-02-21 13:06:34	completed	8	\N	18	1
48	2025-03-03 23:52:22	55	2025-03-03 04:31:16	completed	6	\N	21	3
51	2025-03-07 10:43:42	20	2025-03-05 11:07:54	completed	2	\N	24	4
52	2023-03-26 15:00:00	60	2023-03-25 18:21:18	canceled	4	\N	25	3
28	2025-03-26 10:15:20	50	2025-03-19 14:05:51	active	4	\N	1	2
42	2025-02-26 13:34:38	10	2025-02-20 13:26:06	completed	3	\N	15	2
34	2023-03-08 09:00:00	80	2023-03-03 23:36:38	canceled	7	\N	7	3
38	2023-03-12 09:30:00	20	2023-03-10 17:25:31	canceled	2	\N	11	1
53	2025-03-29 11:24:44	35	2025-03-28 14:29:08	active	7	\N	26	2
\.


--
-- TOC entry 4933 (class 0 OID 25748)
-- Dependencies: 218
-- Data for Name: equipment; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.equipment (id, name, type, brand, model, condition, id_rehearsal_point) FROM stdin;
1	Combo Amplifier	Amplifier	Fender	Champion 100	New	53
2	Combo Amplifier	Amplifier	Fender	Champion 100	New	53
3	Combo Amplifier	Amplifier	Marshall	MG30GFX	Used	53
4	Drum Kit	Drum Set	Pearl	Export Series	Used	53
5	Drum Kit	Drum Set	Tama	Starclassic	New	53
6	Synthesizer	Synthesizer	Yamaha	MX61	New	53
7	Bass Rental	Instrument	Fender	Precision Bass	New	53
8	Combo Amplifier	Amplifier	Roland	CUBE-40GX	New	54
9	Combo Amplifier	Amplifier	Orange	Crush Pro	Used	54
10	Drum Kit	Drum Set	Yamaha	Stage Custom	New	54
11	Combo Amplifier	Amplifier	Fender	Champion 100	New	55
12	Combo Amplifier	Amplifier	Marshall	MG30GFX	Used	55
13	Drum Kit	Drum Set	Pearl	Export Series	Used	55
14	Drum Kit	Drum Set	Tama	Starclassic	New	55
15	Synthesizer	Synthesizer	Yamaha	MX61	New	55
16	Bass Rental	Instrument	Fender	Precision Bass	New	56
17	Combo Amplifier	Amplifier	Roland	CUBE-40GX	New	57
18	Combo Amplifier	Amplifier	Orange	Crush Pro	Used	57
19	Drum Kit	Drum Set	Yamaha	Stage Custom	New	57
20	Drum Kit	Drum Set	Mapex	Armory	Used	58
21	Synthesizer	Synthesizer	Nord	Electro 5	New	58
22	Bass Rental	Instrument	Epiphone	EB-3	New	58
23	Combo Amplifier	Amplifier	Roland	AC30	New	59
24	Combo Amplifier	Amplifier	Vox	Pathfinder	Used	59
25	Drum Kit	Drum Set	Tama	Starclassic	New	59
26	Drum Kit	Drum Set	Yamaha	Live Custom	New	59
27	Synthesizer	Synthesizer	Korg	Volca FM	New	59
28	Bass Rental	Instrument	Fender	Jazz Bass	New	59
29	Combo Amplifier	Amplifier	Bugera	V55	New	60
30	Combo Amplifier	Amplifier	Laney	Ironheart	Used	60
31	Drum Kit	Drum Set	Gretsch	New Classic	New	60
32	Drum Kit	Drum Set	DW	Design Series	Used	60
33	Synthesizer	Synthesizer	Roland	Juno-DS	New	60
34	Bass Rental	Instrument	Ibanez	SR300	New	60
35	Combo Amplifier	Amplifier	Marshall	Code 25	New	61
36	Combo Amplifier	Amplifier	Orange	Tiny Terror	Used	61
37	Drum Kit	Drum Set	Ludwig	Super Classic	New	61
38	Drum Kit	Drum Set	Pearl	Vision	Used	61
39	Synthesizer	Synthesizer	Yamaha	Reface DX	New	61
40	Bass Rental	Instrument	Squier	Affinity	New	61
41	Combo Amplifier	Amplifier	Fender	Frontman	New	62
42	Combo Amplifier	Amplifier	Vox	AC4	Used	62
44	Drum Kit	Drum Set	Tama	S.L.P.	Used	62
45	Synthesizer	Synthesizer	Korg	Monologue	New	62
46	Bass Rental	Instrument	Music Man	Bongo	New	62
43	Drum Kit	Drum Set	Mapex	M Series	New	62
\.


--
-- TOC entry 4938 (class 0 OID 25779)
-- Dependencies: 223
-- Data for Name: equipment_booking; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.equipment_booking (id_equipment, id_booking) FROM stdin;
1	28
2	29
3	30
4	31
5	32
6	33
7	34
8	35
9	36
10	37
11	38
12	39
13	40
14	41
15	42
16	43
17	44
18	45
19	46
20	47
21	48
22	49
23	50
24	51
25	52
26	53
27	54
5	28
8	30
14	34
19	50
19	45
\.


--
-- TOC entry 4932 (class 0 OID 25741)
-- Dependencies: 217
-- Data for Name: rehearsal_points; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.rehearsal_points (id, rating, contact_number, schedule, name, address) FROM stdin;
74	4.4	+375 29 4445566	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Studio 77	Minsk, K. Marksa St., 20
75	4.6	+375 29 5556677	{"Monday": "10:00-22:00", "Tuesday": "10:00-20:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Sound Lab	Minsk, Hikalo St., 3
76	4.5	+375 29 6667788	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-24:00", "Friday": "10:00-22:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Vibe Studio	Minsk, Zmitroka Biaduli St., 8
77	4.2	+375 29 7778899	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-24:00", "Friday": "10:00-22:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Rhythm Studio	Minsk, Bohdanovicha St., 30
78	4.3	+375 29 8889900	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Echo Sound	Minsk, October St., 10
79	4.4	+375 29 9990011	{"Monday": "10:00-22:00", "Tuesday": "10:00-20:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Harmony Sound	Minsk, Lenina St., 25
80	4.5	+375 29 0001122	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-24:00", "Friday": "10:00-22:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Beat Studio	Minsk, Mashinostroiteley St., 15
82	4.3	+375 29 2223344	{"Monday": "10:00-22:00", "Tuesday": "10:00-20:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Vocal Studio	Minsk, Hikalo St., 12
83	4.4	+375 29 3334455	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Sound Wave	Minsk, Mashinostroiteley St., 18
81	4.6	+375 29 1112233	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Music Studio	Minsk, Very Khoruzhey St., 5
53	4.5	+375 29 1504505	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-24:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Black Line Studio	Minsk, Fabriziusa 8
54	4.8	+375 29 5714120	{"Monday": "09:00-21:00", "Tuesday": "09:00-21:00", "Wednesday": "09:00-21:00", "Thursday": "09:00-21:00", "Friday": "09:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	BooM Studio	Minsk, Lenina St., 4
55	4.6	+375 33 3036355	{"Monday": "10:00-22:00", "Tuesday": "10:00-20:00", "Wednesday": "10:00-24:00", "Thursday": "10:00-22:00", "Friday": "10:00-22:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Raga Doo Music Studio	Minsk, Very Khoruzhey St., 29
56	4.7	+375 44 7469444	{"Monday": "11:00-23:00", "Tuesday": "11:00-23:00", "Wednesday": "11:00-24:00", "Thursday": "11:00-23:00", "Friday": "11:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Studio 9	Minsk, Hikalo St., 9, 4th building BGUIR
57	4.5	+375 29 2939033	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Black Beauty Studio	Minsk, Korolya St., 2
58	4.3	+375 29 7925455	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	MetraMusic	Minsk, Danila Serdicha St., 30A
59	4.6	+375 29 7608134	{"Monday": "10:00-24:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	LIVEBOX	Minsk, Masherova Ave., 11
60	4.4	+375 29 7608135	{"Monday": "09:00-21:00", "Tuesday": "09:00-21:00", "Wednesday": "09:00-22:00", "Thursday": "09:00-22:00", "Friday": "09:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Chilli Records	Minsk, Zmitroka Biaduli St., 13
61	4.2	+375 29 5714121	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	JAM Studio (BooM)	Minsk, K. Marksa St., 50A
62	4.5	+375 25 9501863	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	BIG AIR STUDIO	Minsk, Bohdanovicha St., 232
63	4.7	+375 25 7792203	{"Monday": "10:00-22:00", "Tuesday": "10:00-24:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Melody Studio	Minsk, Korolya St., 2/11
64	4.6	+375 29 5528552	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-24:00", "Sunday": "10:00-20:00"}	Sunset	Minsk, Mashinostroiteley St.
65	4.5	+375 29 2574410	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	OVERTONE Studio	Minsk, Korolya St., 2
66	4.8	+375 25 7151359	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	JKBX Studio	Minsk, Masherova Ave., 11, 5th Floor
67	4.3	+375 29 3364499	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Microfon	Minsk, Tolbukhina St., 2, office 13-5
68	4.5	+375 29 1504506	{"Monday": "10:00-22:00", "Tuesday": "10:00-24:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	New Sound Studio	Minsk, Masherova Ave., 10
69	4.4	+375 29 1234567	{"Monday": "09:00-21:00", "Tuesday": "09:00-21:00", "Wednesday": "09:00-22:00", "Thursday": "09:00-22:00", "Friday": "09:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Sound Factory	Minsk, Zmitroka Biaduli St., 1
70	4.2	+375 29 7654321	{"Monday": "11:00-23:00", "Tuesday": "11:00-23:00", "Wednesday": "11:00-24:00", "Thursday": "11:00-23:00", "Friday": "11:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Studio 55	Minsk, K. Marksa St., 12
71	4.1	+375 29 1112233	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Music Hall	Minsk, Bohdanovicha St., 15
72	4.3	+375 29 2223344	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Harmony Studio	Minsk, Lenina St., 14
73	4.5	+375 29 3334455	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Echo Studio	Minsk, October St., 5
\.


--
-- TOC entry 4934 (class 0 OID 25755)
-- Dependencies: 219
-- Data for Name: rooms; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.rooms (id, name, air_conditioner, price, recording_support, area, id_rehearsal_point) FROM stdin;
2	Medium Room	t	40	t	25	53
3	Small Room	f	30	f	15	53
6	Green Room	t	25	f	28	54
7	Large Room	t	50	t	35	55
8	Medium Room	t	45	t	25	55
9	Small Room	f	20	f	12	55
10	Yellow Room	t	50	t	30	56
11	Orange Room	f	35	t	18	56
13	Large Room	t	50	t	35	57
14	Medium Room	t	30	t	26	57
15	Small Room	f	15	f	14	57
16	Black Room	t	45	t	29	58
17	White Room	f	25	t	19	58
18	Gray Room	t	20	f	27	58
19	Large Room	t	50	t	35	59
20	Medium Room	t	40	t	24	59
21	Small Room	f	10	f	13	59
22	Cyan Room	t	35	t	31	60
23	Magenta Room	f	30	t	21	60
24	Brown Room	t	45	f	23	60
25	Large Room	t	50	t	35	61
26	Medium Room	t	40	t	27	61
27	Small Room	f	15	f	11	61
28	Light Blue Room	t	25	t	30	62
29	Light Green Room	f	20	t	20	62
30	Dark Room	t	30	f	18	62
31	Meeting Room	t	35	t	22	63
32	Rehearsal Room	f	10	f	14	63
1	Large Room	f	50	t	35	53
4	Blue Room	f	45	t	30	54
33	Big Room	t	35	f	20	63
12	Purple Room	f	40	f	22	56
34	Small Room	f	30	f	15	68
5	Red Room	t	35	t	20	54
35	Small Room	t	15	f	15	64
\.


--
-- TOC entry 4940 (class 0 OID 25787)
-- Dependencies: 225
-- Data for Name: service; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.service (id, name, price, type, requirements, id_rehearsal_point) FROM stdin;
1	Synthesizer Rental	30	Rental	None	53
3	Combo Amplifier Rental	40	Rental	None	53
4	Mixing Service	150	Recording	DAW, audio interface, monitors	54
5	Mastering Service	200	Recording	High-quality monitors, specialized software	54
6	Sound Recording	180	Recording	Microphones, audio interface, soundproofing	54
7	Vocal Recording	150	Recording	Microphones, pop filter, headphones	55
8	Guitar Rental	20	Rental	None	55
9	Drum Kit Rental	35	Rental	None	55
10	Keyboard Rental	30	Rental	None	56
11	Mixing Service	160	Recording	DAW, audio interface, monitors	56
12	Live Sound Engineering	200	Recording	PA system, microphones, mixing console	56
13	Sound Design	180	Recording	Synthesizers, effects, DAW	57
14	Acoustic Treatment	50	Rental	Acoustic panels, bass traps	57
15	Microphone Rental	15	Rental	None	58
16	Piano Rental	40	Rental	None	58
17	Bass Guitar Rental	25	Rental	None	58
19	Podcast Recording	150	Recording	Microphones, soundproofing	59
20	Home Studio Setup	200	Recording	Monitors, audio interface, software	59
22	Drum Recording	160	Recording	Microphones, soundproofing	60
23	Bass Mixing	170	Recording	DAW, effects plugins	60
24	Guitar Mixing	150	Recording	DAW, effects plugins	61
25	Loop Rental	10	Rental	None	61
26	Audio Interface Rental	25	Rental	None	62
28	Sample Library Access	20	Rental	None	62
18	Mixing Service	120	Recording	DAW, editing software	59
21	Mixing Service	180	Recording	DAW, effects plugins	60
27	Mixing Service	150	Recording	Synthesizer, DAW	62
2	Bass Rental	30	Rental	The bass player	54
29	Mixing Service	35	Recording	DAW, audio interface, monitors	62
30	Drum Recording	45	Recording	Drum Kit	63
\.


--
-- TOC entry 4937 (class 0 OID 25776)
-- Dependencies: 222
-- Data for Name: service_booking; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.service_booking (id_service, id_booking) FROM stdin;
1	28
2	29
3	30
4	31
5	32
6	33
7	34
8	35
9	36
10	37
11	38
12	39
13	40
14	41
15	42
16	43
17	44
18	45
19	46
20	47
21	48
22	49
24	51
25	52
26	53
27	54
3	28
9	30
17	34
22	50
8	31
22	51
25	39
23	50
21	50
\.


--
-- TOC entry 4935 (class 0 OID 25762)
-- Dependencies: 220
-- Data for Name: staff; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.staff (id, full_name, address, experience, phone, age, id_rehearsal_point) FROM stdin;
1	Alexey Petrov	Minsk, Lenin St., 1	5	+375291234567	30	53
2	Darya Ivanova	Minsk, Nezavisimosti Ave., 10	7	+375291234568	35	54
3	Sergey Sokolov	Minsk, Mira St., 5	4	+375291234569	28	55
6	Olga Smirnova	Minsk, Chekistov St., 30	8	+375291234572	40	58
7	Vladimir Ivanov	Minsk, Gorkogo St., 12	5	+375291234573	29	59
8	Maria Orlova	Minsk, Lenin St., 25	4	+375291234574	31	60
9	Andrey Nikiforov	Minsk, Pushkina St., 10	6	+375291234575	34	61
12	Kirill Fedorov	Minsk, Kirova St., 11	2	+375291234578	25	53
13	Yulia Alexandrova	Minsk, Frunze St., 22	7	+375291234579	38	54
16	Dmitry Tikhonov	Minsk, Gogolya St., 8	4	+375291234582	30	57
18	Alexandra Guseva	Minsk, Revolyutsii St., 9	3	+375291234584	28	59
19	Viktor Petrovich	Minsk, Pervomayskaya St., 14	5	+375291234585	29	60
21	Igor Morozov	Minsk, Obronaya St., 19	2	+375291234587	24	62
23	Natalya Ivanenko	Minsk, Karl Marx St., 12	4	+375291234589	26	53
25	Sofia Novik	Minsk, Gagarina St., 33	9	+375291234591	41	55
26	Roman Stepanov	Minsk, Koval St., 16	3	+375291234592	27	56
27	Igor Lebedev	Minsk, Gagarina St., 10	3	+375291234604	28	63
28	Maria Morozova	Minsk, Frunze St., 25	2	+375291234611	24	56
4	Natalia Kuznetsova	Minsk, Pobedy Ave., 20	6	+375291234570	32	55
5	Igor Moroz	Minsk, Sovetskaya St., 15	3	+375291234571	26	55
10	Svetlana Pavlova	Minsk, Stalingrad St., 50	5	+375291234576	33	61
11	Antonina Sergeeva	Minsk, Chernomorskaya St., 45	9	+375291234577	36	54
14	Mikhail Gromov	Minsk, Michurina St., 32	5	+375291234580	27	54
15	Elena Romanova	Minsk, Lenin St., 3	10	+375291234581	42	61
17	Tamara Ivanova	Minsk, Gagarina St., 6	6	+375291234583	37	53
20	Anastasia Semyonova	Minsk, Lenin St., 17	8	+375291234586	39	58
22	Valentina Denisova	Minsk, Sovetskaya St., 7	6	+375291234588	33	58
24	Alexander Zaytsev	Minsk, Kuybysheva St., 28	5	+375291234590	30	53
\.


--
-- TOC entry 4936 (class 0 OID 25769)
-- Dependencies: 221
-- Data for Name: users; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.users (id, full_name, phone, email, registration_date) FROM stdin;
9	Andrey Nikiforov	+375331234575	andrey.nikiforov@gmail.com	2025-06-30 01:37:53
17	Alexandra Guseva	+375331234583	alexandra.guseva@gmail.com	2022-05-20 15:21:19
1	Alexey Petrov	+375291234567	alexey.petrov@gmail.com	2024-07-07 03:07:28
5	Igor Moroz	+375291234571	igor.moroz@gmail.com	2025-11-14 05:43:53
7	Vladimir Ivanov	+375291234573	vladimir.ivanov@gmail.com	2024-08-22 14:39:58
8	Maria Orlova	+375291234574	maria.orlova@gmail.com	2022-09-29 13:36:02
10	Svetlana Pavlova	+375291234576	svetlana.pavlova@gmail.com	2023-09-11 18:24:01
11	Antonina Sergeeva	+375291234577	antonina.sergeeva@gmail.com	2025-03-15 08:30:12
13	Yulia Alexandrova	+375291234579	yulia.alexandrova@gmail.com	2022-09-08 20:17:59
16	Dmitry Tikhonov	+375291234582	dmitry.tikhonov@gmail.com	2024-04-25 21:49:09
20	Maxim Frolov	+375291234600	maxim.frolov@gmail.com	2022-08-13 14:06:00
23	Tatiana Sergeyeva	+375291234603	tatiana.sergeyeva@gmail.com	2023-04-27 21:26:28
24	Igor Lebedev	+375291234604	igor.lebedev@gmail.com	2025-07-28 07:34:59
25	Svetlana Lebedeva	+375291234605	svetlana.lebedeva@gmail.com	2023-06-02 22:13:34
30	Andrey Morozov	+375291234610	andrey.morozov@gmail.com	2022-03-03 06:41:55
31	Maria Morozova	+375291234611	maria.morozova@gmail.com	2023-07-07 07:15:17
4	Natalia Kuznetsova	+375291234570	natalia.kuznetsova@outlook.com	2023-11-15 14:02:59
6	Olga Smirnova	+375291234572	olga.smirnova@mail.ru	2024-11-02 08:54:43
14	Mikhail Gromov	+375291234580	mikhail.gromov@vk.com	2023-01-09 00:17:56
15	Elena Romanova	+375291234581	elena.romanova@mail.ru	2024-08-23 11:02:37
22	Dmitry Sergeyev	+375291234602	dmitry.sergeyev@inbox.ru	2024-06-27 06:54:58
21	Anna Frolova	+375331234601	anna.frolova@vk.com	2025-07-05 22:37:58
2	Darya Ivanova	+375331234568	darya.ivanova@mail.ru	2023-11-26 13:57:36
29	Olga Petrova	+375331234609	olga.petrova@gmail.com	2023-01-19 00:16:13
18	Viktor Petrovich	+375331234584	viktor.petrovich@gmail.com	2023-05-29 01:30:58
3	Sergey Sokolov	+375331234569	sergey.sokolov@outlook.com	2022-03-14 13:44:49
12	Kirill Fedorov	+375441234578	kirill.fedorov@inbox.ru	2024-04-06 17:39:43
19	Anastasia Semyonova	+375441234585	anastasia.semyonova@mail.ru	2022-07-13 10:38:10
26	Yuri Smirnov	+375331234606	yuri.smirnov@gmail.com	2022-06-27 01:35:44
28	Vladimir Petrov	+375331234608	vladimir.petrov@gmail.com	2025-05-31 15:55:49
27	Elena Smirnova	+375291234607	elena.smirnova@mail.ru	2025-11-18 23:21:17
\.


--
-- TOC entry 4784 (class 2606 OID 25786)
-- Name: booking booking_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.booking
    ADD CONSTRAINT booking_pkey PRIMARY KEY (id);


--
-- TOC entry 4776 (class 2606 OID 25754)
-- Name: equipment equipment_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment
    ADD CONSTRAINT equipment_pkey PRIMARY KEY (id);


--
-- TOC entry 4774 (class 2606 OID 25747)
-- Name: rehearsal_points rehearsal_points_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rehearsal_points
    ADD CONSTRAINT rehearsal_points_pkey PRIMARY KEY (id);


--
-- TOC entry 4778 (class 2606 OID 25761)
-- Name: rooms rooms_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rooms
    ADD CONSTRAINT rooms_pkey PRIMARY KEY (id);


--
-- TOC entry 4786 (class 2606 OID 25793)
-- Name: service service_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service
    ADD CONSTRAINT service_pkey PRIMARY KEY (id);


--
-- TOC entry 4780 (class 2606 OID 25768)
-- Name: staff staff_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.staff
    ADD CONSTRAINT staff_pkey PRIMARY KEY (id);


--
-- TOC entry 4782 (class 2606 OID 25775)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


-- Completed on 2025-04-06 01:23:40

--
-- PostgreSQL database dump complete
--

