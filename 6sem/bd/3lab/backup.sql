--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-02-25 02:12:58

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
-- TOC entry 5 (class 2615 OID 16927)
-- Name: main; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA main;


ALTER SCHEMA main OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 234 (class 1259 OID 16985)
-- Name: booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.booking (
    id integer NOT NULL,
    "time" timestamp without time zone NOT NULL,
    cost integer NOT NULL,
    creation_date timestamp without time zone NOT NULL,
    status character varying(50) NOT NULL,
    number_of_people integer NOT NULL,
    id_room integer,
    id_user integer,
    duration integer
);


ALTER TABLE main.booking OWNER TO postgres;

--
-- TOC entry 233 (class 1259 OID 16984)
-- Name: booking_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.booking_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.booking_id_seq OWNER TO postgres;

--
-- TOC entry 4987 (class 0 OID 0)
-- Dependencies: 233
-- Name: booking_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.booking_id_seq OWNED BY main.booking.id;


--
-- TOC entry 228 (class 1259 OID 16958)
-- Name: equipment; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.equipment (
    id integer NOT NULL,
    name text NOT NULL,
    type text NOT NULL,
    brand text NOT NULL,
    model text NOT NULL,
    condition text NOT NULL,
    id_rehearsal_point integer
);


ALTER TABLE main.equipment OWNER TO postgres;

--
-- TOC entry 236 (class 1259 OID 16996)
-- Name: equipment_booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.equipment_booking (
    id_equipment integer NOT NULL,
    id_booking integer NOT NULL
);


ALTER TABLE main.equipment_booking OWNER TO postgres;

--
-- TOC entry 227 (class 1259 OID 16957)
-- Name: equipment_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.equipment_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.equipment_id_seq OWNER TO postgres;

--
-- TOC entry 4988 (class 0 OID 0)
-- Dependencies: 227
-- Name: equipment_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.equipment_id_seq OWNED BY main.equipment.id;


--
-- TOC entry 222 (class 1259 OID 16929)
-- Name: rehearsal_points; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.rehearsal_points (
    id integer NOT NULL,
    rating real,
    contact_number character varying(15) NOT NULL,
    schedule json,
    name text NOT NULL,
    address text NOT NULL
);


ALTER TABLE main.rehearsal_points OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16928)
-- Name: rehearsal_points_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.rehearsal_points_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.rehearsal_points_id_seq OWNER TO postgres;

--
-- TOC entry 4989 (class 0 OID 0)
-- Dependencies: 221
-- Name: rehearsal_points_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.rehearsal_points_id_seq OWNED BY main.rehearsal_points.id;


--
-- TOC entry 224 (class 1259 OID 16938)
-- Name: rooms; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.rooms (
    id integer NOT NULL,
    name text NOT NULL,
    air_conditioner boolean DEFAULT false NOT NULL,
    price integer NOT NULL,
    recording_support boolean DEFAULT false NOT NULL,
    area integer NOT NULL,
    id_rehearsal_point integer
);


ALTER TABLE main.rooms OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16937)
-- Name: rooms_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.rooms_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.rooms_id_seq OWNER TO postgres;

--
-- TOC entry 4990 (class 0 OID 0)
-- Dependencies: 223
-- Name: rooms_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.rooms_id_seq OWNED BY main.rooms.id;


--
-- TOC entry 226 (class 1259 OID 16949)
-- Name: service; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.service (
    id integer NOT NULL,
    name text NOT NULL,
    price integer NOT NULL,
    type text NOT NULL,
    requirements text,
    id_rehearsal_point integer
);


ALTER TABLE main.service OWNER TO postgres;

--
-- TOC entry 235 (class 1259 OID 16991)
-- Name: service_booking; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.service_booking (
    id_service integer NOT NULL,
    id_booking integer NOT NULL
);


ALTER TABLE main.service_booking OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 16948)
-- Name: service_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.service_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.service_id_seq OWNER TO postgres;

--
-- TOC entry 4991 (class 0 OID 0)
-- Dependencies: 225
-- Name: service_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.service_id_seq OWNED BY main.service.id;


--
-- TOC entry 230 (class 1259 OID 16967)
-- Name: staff; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.staff (
    id integer NOT NULL,
    full_name text NOT NULL,
    address text,
    experience integer,
    phone character varying(15) NOT NULL,
    age integer NOT NULL,
    id_rehearsal_point integer
);


ALTER TABLE main.staff OWNER TO postgres;

--
-- TOC entry 229 (class 1259 OID 16966)
-- Name: staff_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.staff_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.staff_id_seq OWNER TO postgres;

--
-- TOC entry 4992 (class 0 OID 0)
-- Dependencies: 229
-- Name: staff_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.staff_id_seq OWNED BY main.staff.id;


--
-- TOC entry 232 (class 1259 OID 16976)
-- Name: users; Type: TABLE; Schema: main; Owner: postgres
--

CREATE TABLE main.users (
    id integer NOT NULL,
    full_name text NOT NULL,
    phone character varying(15) NOT NULL,
    email text NOT NULL,
    registration_date timestamp without time zone NOT NULL
);


ALTER TABLE main.users OWNER TO postgres;

--
-- TOC entry 231 (class 1259 OID 16975)
-- Name: users_id_seq; Type: SEQUENCE; Schema: main; Owner: postgres
--

CREATE SEQUENCE main.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE main.users_id_seq OWNER TO postgres;

--
-- TOC entry 4993 (class 0 OID 0)
-- Dependencies: 231
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: main; Owner: postgres
--

ALTER SEQUENCE main.users_id_seq OWNED BY main.users.id;


--
-- TOC entry 4792 (class 2604 OID 16988)
-- Name: booking id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.booking ALTER COLUMN id SET DEFAULT nextval('main.booking_id_seq'::regclass);


--
-- TOC entry 4789 (class 2604 OID 16961)
-- Name: equipment id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment ALTER COLUMN id SET DEFAULT nextval('main.equipment_id_seq'::regclass);


--
-- TOC entry 4784 (class 2604 OID 16932)
-- Name: rehearsal_points id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rehearsal_points ALTER COLUMN id SET DEFAULT nextval('main.rehearsal_points_id_seq'::regclass);


--
-- TOC entry 4785 (class 2604 OID 16941)
-- Name: rooms id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rooms ALTER COLUMN id SET DEFAULT nextval('main.rooms_id_seq'::regclass);


--
-- TOC entry 4788 (class 2604 OID 16952)
-- Name: service id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service ALTER COLUMN id SET DEFAULT nextval('main.service_id_seq'::regclass);


--
-- TOC entry 4790 (class 2604 OID 16970)
-- Name: staff id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.staff ALTER COLUMN id SET DEFAULT nextval('main.staff_id_seq'::regclass);


--
-- TOC entry 4791 (class 2604 OID 16979)
-- Name: users id; Type: DEFAULT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.users ALTER COLUMN id SET DEFAULT nextval('main.users_id_seq'::regclass);


--
-- TOC entry 4979 (class 0 OID 16985)
-- Dependencies: 234
-- Data for Name: booking; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.booking (id, "time", cost, creation_date, status, number_of_people, id_room, id_user, duration) FROM stdin;
28	2023-03-01 09:00:00	50	2023-02-28 10:00:00	active	4	1	1	2
29	2023-03-05 10:30:00	30	2023-03-04 11:00:00	canceled	2	2	2	1
30	2023-03-03 11:15:00	70	2023-03-02 12:00:00	completed	6	3	3	3
31	2023-03-07 14:00:00	25	2023-03-06 13:00:00	active	3	4	4	1
32	2023-03-02 15:45:00	45	2023-03-01 14:30:00	active	5	5	5	4
33	2023-03-04 08:30:00	15	2023-03-03 15:00:00	completed	1	6	6	2
34	2023-03-08 09:00:00	80	2023-03-07 16:00:00	canceled	7	7	7	3
35	2023-03-09 10:00:00	60	2023-03-08 17:00:00	active	8	8	8	1
36	2023-03-10 12:30:00	40	2023-03-09 18:00:00	completed	4	9	9	2
37	2023-03-11 14:15:00	55	2023-03-10 19:00:00	active	3	10	10	3
38	2023-03-12 09:30:00	20	2023-03-11 09:15:00	canceled	2	11	11	1
39	2023-03-13 11:00:00	35	2023-03-12 10:15:00	completed	6	12	12	2
40	2023-03-14 13:30:00	65	2023-03-13 11:15:00	active	5	13	13	4
41	2023-03-15 15:00:00	50	2023-03-14 12:15:00	active	1	14	14	3
42	2023-03-16 16:45:00	10	2023-03-15 13:15:00	completed	3	15	15	2
43	2023-03-17 10:00:00	75	2023-03-16 14:15:00	canceled	2	16	16	1
44	2023-03-18 11:30:00	45	2023-03-17 15:15:00	active	4	17	17	3
45	2023-03-19 12:00:00	30	2023-03-18 16:15:00	completed	8	18	18	1
46	2023-03-20 08:15:00	65	2023-03-19 17:15:00	active	7	19	19	2
47	2023-03-21 09:45:00	15	2023-03-20 18:15:00	canceled	1	20	20	4
48	2023-03-22 10:30:00	55	2023-03-21 19:15:00	completed	6	21	21	3
49	2023-03-23 11:00:00	80	2023-03-22 09:00:00	active	5	22	22	1
50	2023-03-24 12:30:00	40	2023-03-23 10:00:00	active	3	23	23	2
51	2023-03-25 14:15:00	20	2023-03-24 11:00:00	completed	2	24	24	4
52	2023-03-26 15:00:00	60	2023-03-25 12:00:00	canceled	4	25	25	3
53	2023-03-27 16:30:00	35	2023-03-26 13:00:00	active	7	26	26	2
54	2023-03-28 17:00:00	50	2023-03-27 14:00:00	completed	8	27	27	1
\.


--
-- TOC entry 4973 (class 0 OID 16958)
-- Dependencies: 228
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
43	Drum Kit	Drum Set	Mapex	M Series	New	62
44	Drum Kit	Drum Set	Tama	S.L.P.	Used	62
45	Synthesizer	Synthesizer	Korg	Monologue	New	62
46	Bass Rental	Instrument	Music Man	Bongo	New	62
\.


--
-- TOC entry 4981 (class 0 OID 16996)
-- Dependencies: 236
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
\.


--
-- TOC entry 4967 (class 0 OID 16929)
-- Dependencies: 222
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
81	4.6	+375 29 1112233	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "10:00-22:00", "Sunday": "10:00-20:00"}	Music Studio	Minsk, Very Khoruzhey St., 5
82	4.3	+375 29 2223344	{"Monday": "10:00-22:00", "Tuesday": "10:00-20:00", "Wednesday": "10:00-22:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Vocal Studio	Minsk, Hikalo St., 12
83	4.4	+375 29 3334455	{"Monday": "10:00-22:00", "Tuesday": "10:00-22:00", "Wednesday": "10:00-20:00", "Thursday": "10:00-22:00", "Friday": "10:00-24:00", "Saturday": "12:00-24:00", "Sunday": "12:00-20:00"}	Sound Wave	Minsk, Mashinostroiteley St., 18
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
-- TOC entry 4969 (class 0 OID 16938)
-- Dependencies: 224
-- Data for Name: rooms; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.rooms (id, name, air_conditioner, price, recording_support, area, id_rehearsal_point) FROM stdin;
1	Large Room	t	50	t	35	53
2	Medium Room	t	40	t	25	53
3	Small Room	f	30	f	15	53
4	Blue Room	t	45	t	30	54
5	Red Room	f	35	t	20	54
6	Green Room	t	25	f	28	54
7	Large Room	t	50	t	35	55
8	Medium Room	t	45	t	25	55
9	Small Room	f	20	f	12	55
10	Yellow Room	t	50	t	30	56
11	Orange Room	f	35	t	18	56
12	Purple Room	t	40	f	22	56
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
\.


--
-- TOC entry 4971 (class 0 OID 16949)
-- Dependencies: 226
-- Data for Name: service; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.service (id, name, price, type, requirements, id_rehearsal_point) FROM stdin;
1	Synthesizer Rental	30	Rental	None	53
2	Bass Rental	25	Rental	None	53
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
18	Editing Service	120	Recording	DAW, editing software	59
19	Podcast Recording	150	Recording	Microphones, soundproofing	59
20	Home Studio Setup	200	Recording	Monitors, audio interface, software	59
21	Vocal Mixing	180	Recording	DAW, effects plugins	60
22	Drum Recording	160	Recording	Microphones, soundproofing	60
23	Bass Mixing	170	Recording	DAW, effects plugins	60
24	Guitar Mixing	150	Recording	DAW, effects plugins	61
25	Loop Rental	10	Rental	None	61
26	Audio Interface Rental	25	Rental	None	62
27	Synth Programming	30	Recording	Synthesizer, DAW	62
28	Sample Library Access	20	Rental	None	62
29	Soundtrack Composition	200	Recording	DAW, instruments	63
30	Live Streaming Setup	150	Recording	Camera, microphone, streaming software	63
\.


--
-- TOC entry 4980 (class 0 OID 16991)
-- Dependencies: 235
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
23	50
24	51
25	52
26	53
27	54
3	28
9	30
17	34
22	50
\.


--
-- TOC entry 4975 (class 0 OID 16967)
-- Dependencies: 230
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
-- TOC entry 4977 (class 0 OID 16976)
-- Dependencies: 232
-- Data for Name: users; Type: TABLE DATA; Schema: main; Owner: postgres
--

COPY main.users (id, full_name, phone, email, registration_date) FROM stdin;
1	Alexey Petrov	+375291234567	alexey.petrov@example.com	2023-01-15 10:00:00
2	Darya Ivanova	+375291234568	darya.ivanova@example.com	2023-01-16 11:15:00
3	Sergey Sokolov	+375291234569	sergey.sokolov@example.com	2023-01-17 12:30:00
4	Natalia Kuznetsova	+375291234570	natalia.kuznetsova@example.com	2023-01-18 13:45:00
5	Igor Moroz	+375291234571	igor.moroz@example.com	2023-01-19 14:00:00
6	Olga Smirnova	+375291234572	olga.smirnova@example.com	2023-01-20 15:10:00
7	Vladimir Ivanov	+375291234573	vladimir.ivanov@example.com	2023-01-21 16:20:00
8	Maria Orlova	+375291234574	maria.orlova@example.com	2023-01-22 17:30:00
9	Andrey Nikiforov	+375291234575	andrey.nikiforov@example.com	2023-01-23 18:40:00
10	Svetlana Pavlova	+375291234576	svetlana.pavlova@example.com	2023-01-24 19:50:00
11	Antonina Sergeeva	+375291234577	antonina.sergeeva@example.com	2023-01-25 20:00:00
12	Kirill Fedorov	+375291234578	kirill.fedorov@example.com	2023-01-26 21:10:00
13	Yulia Alexandrova	+375291234579	yulia.alexandrova@example.com	2023-01-27 22:20:00
14	Mikhail Gromov	+375291234580	mikhail.gromov@example.com	2023-01-28 23:30:00
15	Elena Romanova	+375291234581	elena.romanova@example.com	2023-01-29 09:00:00
16	Dmitry Tikhonov	+375291234582	dmitry.tikhonov@example.com	2023-01-30 10:05:00
17	Alexandra Guseva	+375291234583	alexandra.guseva@example.com	2023-01-31 11:15:00
18	Viktor Petrovich	+375291234584	viktor.petrovich@example.com	2023-02-01 12:20:00
19	Anastasia Semyonova	+375291234585	anastasia.semyonova@example.com	2023-02-02 13:25:00
20	Maxim Frolov	+375291234600	maxim.frolov@example.com	2023-02-03 14:30:00
21	Anna Frolova	+375291234601	anna.frolova@example.com	2023-02-04 15:35:00
22	Dmitry Sergeyev	+375291234602	dmitry.sergeyev@example.com	2023-02-05 16:40:00
23	Tatiana Sergeyeva	+375291234603	tatiana.sergeyeva@example.com	2023-02-06 17:45:00
24	Igor Lebedev	+375291234604	igor.lebedev@example.com	2023-02-07 18:50:00
25	Svetlana Lebedeva	+375291234605	svetlana.lebedeva@example.com	2023-02-08 19:55:00
26	Yuri Smirnov	+375291234606	yuri.smirnov@example.com	2023-02-09 20:00:00
27	Elena Smirnova	+375291234607	elena.smirnova@example.com	2023-02-10 21:05:00
28	Vladimir Petrov	+375291234608	vladimir.petrov@example.com	2023-02-11 22:10:00
29	Olga Petrova	+375291234609	olga.petrova@example.com	2023-02-12 23:15:00
30	Andrey Morozov	+375291234610	andrey.morozov@example.com	2023-02-13 09:20:00
31	Maria Morozova	+375291234611	maria.morozova@example.com	2023-02-14 10:25:00
\.


--
-- TOC entry 4994 (class 0 OID 0)
-- Dependencies: 233
-- Name: booking_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.booking_id_seq', 54, true);


--
-- TOC entry 4995 (class 0 OID 0)
-- Dependencies: 227
-- Name: equipment_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.equipment_id_seq', 48, true);


--
-- TOC entry 4996 (class 0 OID 0)
-- Dependencies: 221
-- Name: rehearsal_points_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.rehearsal_points_id_seq', 83, true);


--
-- TOC entry 4997 (class 0 OID 0)
-- Dependencies: 223
-- Name: rooms_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.rooms_id_seq', 32, true);


--
-- TOC entry 4998 (class 0 OID 0)
-- Dependencies: 225
-- Name: service_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.service_id_seq', 30, true);


--
-- TOC entry 4999 (class 0 OID 0)
-- Dependencies: 229
-- Name: staff_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.staff_id_seq', 28, true);


--
-- TOC entry 5000 (class 0 OID 0)
-- Dependencies: 231
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: main; Owner: postgres
--

SELECT pg_catalog.setval('main.users_id_seq', 31, true);


--
-- TOC entry 4806 (class 2606 OID 16990)
-- Name: booking booking_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.booking
    ADD CONSTRAINT booking_pkey PRIMARY KEY (id);


--
-- TOC entry 4810 (class 2606 OID 17000)
-- Name: equipment_booking equipment_booking_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment_booking
    ADD CONSTRAINT equipment_booking_pkey PRIMARY KEY (id_equipment, id_booking);


--
-- TOC entry 4800 (class 2606 OID 16965)
-- Name: equipment equipment_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment
    ADD CONSTRAINT equipment_pkey PRIMARY KEY (id);


--
-- TOC entry 4794 (class 2606 OID 16936)
-- Name: rehearsal_points rehearsal_points_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rehearsal_points
    ADD CONSTRAINT rehearsal_points_pkey PRIMARY KEY (id);


--
-- TOC entry 4796 (class 2606 OID 16947)
-- Name: rooms rooms_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rooms
    ADD CONSTRAINT rooms_pkey PRIMARY KEY (id);


--
-- TOC entry 4808 (class 2606 OID 16995)
-- Name: service_booking service_booking_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service_booking
    ADD CONSTRAINT service_booking_pkey PRIMARY KEY (id_service, id_booking);


--
-- TOC entry 4798 (class 2606 OID 16956)
-- Name: service service_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service
    ADD CONSTRAINT service_pkey PRIMARY KEY (id);


--
-- TOC entry 4802 (class 2606 OID 16974)
-- Name: staff staff_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.staff
    ADD CONSTRAINT staff_pkey PRIMARY KEY (id);


--
-- TOC entry 4804 (class 2606 OID 16983)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 4815 (class 2606 OID 17021)
-- Name: booking fk_booking_room; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.booking
    ADD CONSTRAINT fk_booking_room FOREIGN KEY (id_room) REFERENCES main.rooms(id) ON DELETE SET NULL;


--
-- TOC entry 4816 (class 2606 OID 17026)
-- Name: booking fk_booking_user; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.booking
    ADD CONSTRAINT fk_booking_user FOREIGN KEY (id_user) REFERENCES main.users(id) ON DELETE SET NULL;


--
-- TOC entry 4819 (class 2606 OID 17046)
-- Name: equipment_booking fk_equipment_booking_booking; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment_booking
    ADD CONSTRAINT fk_equipment_booking_booking FOREIGN KEY (id_booking) REFERENCES main.booking(id) ON DELETE SET NULL;


--
-- TOC entry 4820 (class 2606 OID 17041)
-- Name: equipment_booking fk_equipment_booking_equipment; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment_booking
    ADD CONSTRAINT fk_equipment_booking_equipment FOREIGN KEY (id_equipment) REFERENCES main.equipment(id) ON DELETE SET NULL;


--
-- TOC entry 4813 (class 2606 OID 17011)
-- Name: equipment fk_equipment_rehearsal_point; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.equipment
    ADD CONSTRAINT fk_equipment_rehearsal_point FOREIGN KEY (id_rehearsal_point) REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;


--
-- TOC entry 4811 (class 2606 OID 17001)
-- Name: rooms fk_rooms_rehearsal_point; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.rooms
    ADD CONSTRAINT fk_rooms_rehearsal_point FOREIGN KEY (id_rehearsal_point) REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;


--
-- TOC entry 4817 (class 2606 OID 17036)
-- Name: service_booking fk_service_booking_booking; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service_booking
    ADD CONSTRAINT fk_service_booking_booking FOREIGN KEY (id_booking) REFERENCES main.booking(id) ON DELETE SET NULL;


--
-- TOC entry 4818 (class 2606 OID 17031)
-- Name: service_booking fk_service_booking_service; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service_booking
    ADD CONSTRAINT fk_service_booking_service FOREIGN KEY (id_service) REFERENCES main.service(id) ON DELETE SET NULL;


--
-- TOC entry 4812 (class 2606 OID 17006)
-- Name: service fk_service_rehearsal_point; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.service
    ADD CONSTRAINT fk_service_rehearsal_point FOREIGN KEY (id_rehearsal_point) REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;


--
-- TOC entry 4814 (class 2606 OID 17016)
-- Name: staff fk_staff_rehearsal_point; Type: FK CONSTRAINT; Schema: main; Owner: postgres
--

ALTER TABLE ONLY main.staff
    ADD CONSTRAINT fk_staff_rehearsal_point FOREIGN KEY (id_rehearsal_point) REFERENCES main.rehearsal_points(id) ON DELETE SET NULL;


-- Completed on 2025-02-25 02:12:59

--
-- PostgreSQL database dump complete
--

