# Database connection

public server = 14.63.184.255:3306

```
> desc questions
+---------------+---------------------+------+-----+---------+----------------+
| Field         | Type                | Null | Key | Default | Extra          |
+---------------+---------------------+------+-----+---------+----------------+
| id            | int(10) unsigned    | NO   | PRI | NULL    | auto_increment |
| exam_id       | int(10) unsigned    | NO   | MUL | NULL    |                |
| number        | tinyint(3) unsigned | NO   |     | NULL    |                |
| correct       | tinyint(3) unsigned | NO   |     | NULL    |                |
| point         | tinyint(3) unsigned | NO   |     | NULL    |                |
| image_path    | varchar(45)         | YES  |     | NULL    |                |
| page_number   | tinyint(3) unsigned | NO   |     | NULL    |                |
| page_column   | tinyint(3)          | NO   |     | NULL    |                |
| passage_array | varchar(45)         | YES  |     |         |                |
| hit           | int(10) unsigned    | NO   |     | 0       |                |
| created_at    | datetime            | NO   |     | NULL    |                |
+---------------+---------------------+------+-----+---------+----------------+
```

`image_path` column이 이미지 경로이고, base_url은 `http://cdn.odab.net/static/img/questions/`입니다.

실제 작업할때는 `exam_id` 기준으로 일정 범위로 잘라서 배치처리하는게 좋을것 같구요.

- - -

`number`는 문제번호, `correct`는 정답번호입니다.

`exam_id`는 외래키인데 `exams` 테이블에는 년, 월, 과목 등 시험정보가 들어있습니다.

http://cdn.odab.net/img/questions/130903/130903_G3_KA_01.png