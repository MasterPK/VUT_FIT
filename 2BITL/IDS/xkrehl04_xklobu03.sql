SET SERVEROUTPUT ON;
DROP TABLE orders CASCADE CONSTRAINTS;
DROP TABLE employee CASCADE CONSTRAINTS;
DROP TABLE car_mechanic CASCADE CONSTRAINTS;
DROP TABLE car CASCADE CONSTRAINTS;
DROP TABLE invoice CASCADE CONSTRAINTS;
DROP TABLE car_vendor CASCADE CONSTRAINTS;
DROP TABLE material CASCADE CONSTRAINTS;
DROP TABLE order_repair CASCADE CONSTRAINTS;
DROP TABLE type_of_repair CASCADE CONSTRAINTS;
DROP TABLE material_repair CASCADE CONSTRAINTS;
DROP TABLE mechanic_repair CASCADE CONSTRAINTS;



CREATE TABLE material
(
    id INTEGER PRIMARY KEY NOT NULL,
    name VARCHAR(255),
    available_amount INTEGER NOT NULL
);



CREATE TABLE employee
(
    id INTEGER PRIMARY KEY NOT NULL,
    first_name VARCHAR2(255) NOT NULL,
    last_name VARCHAR2(255) NOT NULL,
    hourly_wage NUMBER (20, 2) NOT NULL,
    personal_identification_number INTEGER UNIQUE NOT NULL,
    CHECK ((LENGTH(personal_identification_number)=9 AND TO_NUMBER(SUBSTR(personal_identification_number,7,3))<>0) OR(LENGTH(personal_identification_number)=10 AND MOD(personal_identification_number,11) = 0)),
    CHECK(TO_NUMBER(SUBSTR(personal_identification_number,1,2))>=0 AND TO_NUMBER(SUBSTR(personal_identification_number,1,2))<=99 ),
    CHECK((TO_NUMBER(SUBSTR(personal_identification_number,3,2))>=1 AND TO_NUMBER(SUBSTR(personal_identification_number,3,2))<=12 )OR( TO_NUMBER(SUBSTR(personal_identification_number,3,2))>=51 AND TO_NUMBER(SUBSTR(personal_identification_number,3,2))<=62)),
    CHECK(TO_NUMBER(SUBSTR(personal_identification_number,5,2))>=1 AND TO_NUMBER(SUBSTR(personal_identification_number,5,2))<=31) 
); 


CREATE TABLE invoice
(
    id INTEGER PRIMARY KEY NOT NULL,
    timestamp_created TIMESTAMP,
    employeee INTEGER,
    CONSTRAINT employeee FOREIGN KEY (employeee) REFERENCES employee(id)
);

CREATE TABLE car_vendor
(
    id INTEGER PRIMARY KEY NOT NULL,
    name VARCHAR(255) NOT NULL,
    supplier VARCHAR(255) NOT NULL
);

CREATE TABLE car
(
    vin INTEGER PRIMARY KEY NOT NULL,
    name VARCHAR(255),
    date_of_made TIMESTAMP,
    radio VARCHAR(255),
    spz VARCHAR(255) NOT NULL,
    color VARCHAR2(255),
    vendor INTEGER NOT NULL,
    CONSTRAINT vendor FOREIGN KEY (vendor) REFERENCES car_vendor(id)
);

CREATE TABLE orders
(
    id INTEGER PRIMARY KEY NOT NULL,
    timestamp_created TIMESTAMP,
    timestamp_expected TIMESTAMP,
    employee INTEGER NOT NULL,
    car INTEGER NOT NULL,
    invoice INTEGER UNIQUE,
    CONSTRAINT invoice FOREIGN KEY (invoice) REFERENCES invoice(id),
    CONSTRAINT employee FOREIGN KEY (employee) REFERENCES employee(id),
    CONSTRAINT car FOREIGN KEY (car) REFERENCES car(vin)
);

CREATE TABLE car_mechanic
(
    specialization VARCHAR(255),
    FK_mechanic INTEGER PRIMARY KEY NOT NULL,
    CONSTRAINT FK_mechanic FOREIGN KEY (FK_mechanic) REFERENCES employee(id)
);

CREATE TABLE type_of_repair
(
    id INTEGER PRIMARY KEY NOT NULL,
    name VARCHAR(255),
    material_cost INTEGER NOT NULL,
    mechanic_time INTEGER NOT NULL
);

CREATE TABLE material_repair
(
    material_id1 INTEGER,
    type_of_repair_id2 INTEGER,
    CONSTRAINT material_id1 FOREIGN KEY (material_id1) REFERENCES material(id),
    CONSTRAINT type_of_repair_id2 FOREIGN KEY (type_of_repair_id2) REFERENCES type_of_repair(id)
);

CREATE TABLE order_repair
(
    order_id INTEGER NOT NULL,
    type_of_repair_id INTEGER NOT NULL,
    CONSTRAINT order_id FOREIGN KEY (order_id) REFERENCES orders(id),
    CONSTRAINT type_of_repair_id FOREIGN KEY (type_of_repair_id) REFERENCES type_of_repair(id)

);

CREATE TABLE mechanic_repair
(
    mechanic_id123 INTEGER NOT NULL,
    type_of_repair_id123 INTEGER NOT NULL,
    CONSTRAINT mechanic_id123 FOREIGN KEY (mechanic_id123) REFERENCES car_mechanic(FK_mechanic),
    CONSTRAINT type_of_repair_id123 FOREIGN KEY (type_of_repair_id123) REFERENCES type_of_repair(id)

);


GRANT ALL ON orders TO xklobu03;
GRANT SELECT ON employee TO xklobu03;
GRANT SELECT ON car_mechanic TO xklobu03;
GRANT ALL ON car TO xklobu03;
GRANT ALL ON invoice TO xklobu03;
GRANT ALL ON car_vendor TO xklobu03;
GRANT ALL ON material TO xklobu03;
GRANT ALL ON order_repair TO xklobu03;
GRANT ALL ON type_of_repair TO xklobu03;
GRANT ALL ON material_repair TO xklobu03;
GRANT SELECT ON mechanic_repair TO xklobu03;


CREATE OR REPLACE TRIGGER primary_key_employee
BEFORE INSERT ON employee
FOR EACH ROW
WHEN (NEW.id IS NULL)
DECLARE
    tmp number;
BEGIN
    SELECT MAX(id) INTO tmp FROM employee;
    IF tmp IS NULL THEN
        :NEW.id:=1;
    ELSE
        :NEW.id:=tmp+1;
    END IF;
    
END;
/

CREATE OR REPLACE TRIGGER primary_key_car_vendor
BEFORE INSERT ON car_vendor
FOR EACH ROW
WHEN (NEW.id IS NULL)
DECLARE
    tmp number;
BEGIN
    SELECT MAX(id) INTO tmp FROM car_vendor;
    IF tmp IS NULL THEN
        :NEW.id:=1;
    ELSE
        :NEW.id:=tmp+1;
    END IF;
    
END;
/

CREATE OR REPLACE TYPE varray is table of number;
/

CREATE OR REPLACE TRIGGER material_check
BEFORE INSERT ON order_repair
FOR EACH ROW
DECLARE
    material_id number;
    material_count number;
    p varray := varray();
    zero_count EXCEPTION;
BEGIN
    for i in (SELECT material_id1 INTO material_id FROM material_repair WHERE type_of_repair_id2=:NEW.type_of_repair_id) loop
        p.extend;
        p(p.count) := i.material_id1;
    end loop;
    for xarr in (select column_value from table(cast(p as varray))) loop
        SELECT available_amount INTO material_count FROM material WHERE id=xarr.column_value;
        if material_count=0 then
            raise zero_count;
        else 
            UPDATE material SET available_amount=available_amount-1 WHERE id=xarr.column_value;
        end if;
    end loop;
EXCEPTION 
   WHEN zero_count THEN 
      dbms_output.put_line('You dont have enough material!'); 
    
END;
/

CREATE OR REPLACE PROCEDURE count_order_by_car_spz ( car_spz IN varchar2, count_order OUT number) IS
    cursor c1 is
    SELECT vin
    FROM car
    WHERE spz = car_spz;
    car_vin car.spz%TYPE;
    zero_count EXCEPTION;
BEGIN
    OPEN c1;
    FETCH c1 INTO car_vin;
    
    SELECT COUNT(*) INTO count_order FROM orders WHERE car=car_vin;
    
    CLOSE c1;
    if count_order=0 then
        raise zero_count;
    end if;
EXCEPTION 
   WHEN zero_count THEN 
      dbms_output.put_line('No order with this car found!'); 
END;
/

CREATE OR REPLACE PROCEDURE insert_update_car_vendor_supplier ( vendor_name IN varchar2, supplier_new IN varchar2) IS
    cursor c1 is
    SELECT id
    FROM car_vendor
    WHERE name = vendor_name;
    car_id car_vendor.name%TYPE;
BEGIN
    OPEN c1;
    FETCH c1 INTO car_id;
    
    IF c1%FOUND then
        UPDATE car_vendor SET supplier=supplier_new WHERE id=car_id;
    else
        INSERT INTO car_vendor(name,supplier) VALUES (vendor_name, supplier_new);
    end if;
    
    CLOSE c1;

END;
/



INSERT INTO car_vendor VALUES (1,'Skoda','Dodavatel 123');
INSERT INTO car_vendor VALUES (2,'VW','Dodavatel 321');
INSERT INTO car_vendor VALUES (3,'Opel','Dodavatel 011');
INSERT INTO car (vin,radio,spz,color,vendor,name,date_of_made) VALUES (1,'ano','TN-95653','black',1,'Octavia',TO_DATE ('20-12-2015', 'DD-MM-YYYY'));
INSERT INTO car (vin,radio,spz,color,vendor,name,date_of_made) VALUES (2,'ano','TN-41043','blue',2,'Golf',TO_DATE ('25-05-2014', 'DD-MM-YYYY'));
INSERT INTO car (vin,radio,spz,color,vendor,name,date_of_made) VALUES (3,'ano','6B4 1548','silver',3,'Zafira',TO_DATE ('30-10-2013', 'DD-MM-YYYY'));
INSERT INTO car (vin,radio,spz,color,vendor,name,date_of_made) VALUES (4,'ano','6B5 8846','silver',1,'Fabia',TO_DATE ('15-02-2017', 'DD-MM-YYYY'));

INSERT INTO employee (id,first_name,last_name,hourly_wage,personal_identification_number) VALUES (NULL,'Petr','Krehlik',300,9801234949);

INSERT INTO employee (id,first_name,last_name,hourly_wage,personal_identification_number) VALUES (NULL,'Martin','Klobusicky',150,981211001);

INSERT INTO employee (first_name,last_name,hourly_wage,personal_identification_number) VALUES ('Matìj','Novák',100,9805268671);

INSERT INTO material(id,name,available_amount) VALUES (1,'Kola',0);
INSERT INTO material(id,name,available_amount) VALUES (2,'Turbo',5);
INSERT INTO material(id,name,available_amount) VALUES (3,'Klimatizace',1);
INSERT INTO material(id,name,available_amount) VALUES (4,'Kapalina klimatizace',10);
INSERT INTO type_of_repair(id,name,material_cost,mechanic_time) VALUES (1,'Vymena kol',5000,3);
INSERT INTO type_of_repair(id,name,material_cost,mechanic_time) VALUES (2,'Vymena klimatizace',2000,1);
INSERT INTO type_of_repair(id,name,material_cost,mechanic_time) VALUES (3,'Vymena turba',10000,2);

INSERT INTO material_repair(material_id1,type_of_repair_id2) VALUES (1,1);
INSERT INTO material_repair(material_id1,type_of_repair_id2) VALUES (4,2);
INSERT INTO material_repair(material_id1,type_of_repair_id2) VALUES (2,3);
INSERT INTO material_repair(material_id1,type_of_repair_id2) VALUES (3,2);

INSERT INTO orders (id,employee,car) VALUES(1,2, 1);
INSERT INTO orders (id,employee,car) VALUES(2,2, 2);
INSERT INTO orders (id,employee,car) VALUES(5,2, 2);
INSERT INTO orders (id,employee,car) VALUES(6,2, 2);
INSERT INTO orders (id,employee,car) VALUES(3,2, 3);
INSERT INTO orders (id,employee,car) VALUES(4 ,2, 3);

INSERT INTO order_repair(order_id,type_of_repair_id) VALUES(1,1);
INSERT INTO order_repair(order_id,type_of_repair_id) VALUES(1,2);
INSERT INTO order_repair(order_id,type_of_repair_id) VALUES(2,3);
INSERT INTO order_repair(order_id,type_of_repair_id) VALUES(2,1);
INSERT INTO order_repair(order_id,type_of_repair_id) VALUES(3,1);

INSERT INTO invoice(id,employeee) VALUES (1,1);
INSERT INTO invoice(id,employeee) VALUES (2,1);
INSERT INTO invoice(id,employeee) VALUES (3,2);
UPDATE orders SET invoice=1 WHERE id=1; 

DECLARE
    count_order1 number := 0;  
BEGIN
--Neexistuje zadna objednavka
count_order_by_car_spz('TN-410453',count_order1);
dbms_output.put_line('Count_order: ' || count_order1); 

--Existuje
count_order_by_car_spz('TN-95653',count_order1);
dbms_output.put_line('Count_order: ' || count_order1); 

--Zmena dodavatele
insert_update_car_vendor_supplier('Skoda','Dodavatel 55555');
--Nova znacka a dodavatel
insert_update_car_vendor_supplier('Audi','Dodavatel 55555');

END;
/
SELECT * FROM car_vendor;

--Plan bez indexu
EXPLAIN PLAN FOR SELECT employee.first_name, employee.last_name, COUNT(*) FROM invoice, employee WHERE invoice.employeee=employee.id GROUP BY employee.first_name,employee.last_name;
SELECT plan_table_output FROM table (dbms_xplan.display());

DROP INDEX indexx;
CREATE INDEX indexx ON invoice(employeee);

--Plan s indexem
EXPLAIN PLAN FOR SELECT employee.first_name, employee.last_name, COUNT(*) FROM invoice, employee WHERE invoice.employeee=employee.id GROUP BY employee.first_name,employee.last_name;
SELECT plan_table_output FROM table (dbms_xplan.display());

--Materializovany pohlec pro druheho uzivatele xklobu03

DROP MATERIALIZED VIEW car_date_of_made;

CREATE MATERIALIZED VIEW car_date_of_made
BUILD IMMEDIATE 
REFRESH ON COMMIT AS 
  SELECT XKREHL04.car.date_of_made AS datum_vyroby
  FROM XKREHL04.car;
  
select * from car_date_of_made;
INSERT INTO XKREHL04.car (vin,radio,spz,color,vendor,name,date_of_made) VALUES (8,'ano','6B9 8846','silver',1,'Superb',TO_DATE ('20-12-2019', 'DD-MM-YYYY'));
select * from car_date_of_made;
COMMIT; 
select * from car_date_of_made;