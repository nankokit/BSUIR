CREATE TABLE IF NOT EXISTS main.rehearsal_points (
id SERIAL PRIMARY KEY,
rating REAL,
contact_number VARCHAR(15) NOT NULL,
schedule JSON,
name TEXT NOT NULL,
address TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS main.rooms (
id SERIAL PRIMARY KEY,
name TEXT NOT NULL,
air_conditioner BOOLEAN NOT NULL DEFAULT FALSE,
price INT NOT NULL,
recording_support BOOLEAN NOT NULL DEFAULT FALSE,
area INT NOT NULL,
id_rehearsal_point INT
);

CREATE TABLE IF NOT EXISTS main.service (
id SERIAL PRIMARY KEY,
name TEXT NOT NULL,
price INT NOT NULL,
type TEXT NOT NULL,
requirements TEXT,
id_rehearsal_point INT
);

CREATE TABLE IF NOT EXISTS main.equipment (
id SERIAL PRIMARY KEY,
name TEXT NOT NULL,
type TEXT NOT NULL,
brand TEXT NOT NULL,
model TEXT NOT NULL,
condition TEXT NOT NULL,
id_rehearsal_point INT
);

CREATE TABLE IF NOT EXISTS main.staff (
id SERIAL PRIMARY KEY,
full_name TEXT NOT NULL,
address TEXT,
experience INT,
phone VARCHAR(15) NOT NULL,
age INT NOT NULL,
id_rehearsal_point INT
);

CREATE TABLE IF NOT EXISTS main.users (
id SERIAL PRIMARY KEY,
full_name TEXT NOT NULL,
phone VARCHAR(15) NOT NULL,
email TEXT NOT NULL,
registration_date TIMESTAMP NOT NULL
);

CREATE TABLE IF NOT EXISTS main.booking (
id SERIAL PRIMARY KEY,
time TIMESTAMP NOT NULL,
duration INT,
cost INT NOT NULL,
creation_date TIMESTAMP NOT NULL,
status VARCHAR(50) NOT NULL,
number_of_people INT NOT NULL,
id_room INT,
id_user INT
);

CREATE TABLE IF NOT EXISTS main.service_booking (
id_service INT NOT NULL,
id_booking INT NOT NULL,
PRIMARY KEY (id_service, id_booking)
);

CREATE TABLE IF NOT EXISTS main.equipment_booking (
id_equipment INT NOT NULL,
id_booking INT NOT NULL,
PRIMARY KEY (id_equipment, id_booking)
);

ALTER TABLE IF EXISTS main.rooms
ADD CONSTRAINT fk_rooms_rehearsal_point FOREIGN KEY (id_rehearsal_point)
REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;

ALTER TABLE IF EXISTS main.service
ADD CONSTRAINT fk_service_rehearsal_point FOREIGN KEY (id_rehearsal_point)
REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;

ALTER TABLE IF EXISTS main.equipment
ADD CONSTRAINT fk_equipment_rehearsal_point FOREIGN KEY (id_rehearsal_point)
REFERENCES main.rehearsal_points(id) ON DELETE CASCADE;

ALTER TABLE IF EXISTS main.staff
ADD CONSTRAINT fk_staff_rehearsal_point FOREIGN KEY (id_rehearsal_point)
REFERENCES main.rehearsal_points(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.booking
ADD CONSTRAINT fk_booking_room FOREIGN KEY (id_room)
REFERENCES main.rooms(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.booking
ADD CONSTRAINT fk_booking_user FOREIGN KEY (id_user)
REFERENCES main.users(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.service_booking
ADD CONSTRAINT fk_service_booking_service FOREIGN KEY (id_service)
REFERENCES main.service(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.service_booking
ADD CONSTRAINT fk_service_booking_booking FOREIGN KEY (id_booking)
REFERENCES main.booking(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.equipment_booking
ADD CONSTRAINT fk_equipment_booking_equipment FOREIGN KEY (id_equipment)
REFERENCES main.equipment(id) ON DELETE SET NULL;

ALTER TABLE IF EXISTS main.equipment_booking
ADD CONSTRAINT fk_equipment_booking_booking FOREIGN KEY (id_booking)
REFERENCES main.booking(id) ON DELETE SET NULL;
