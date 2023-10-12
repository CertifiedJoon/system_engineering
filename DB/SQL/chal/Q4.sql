select cnt.name as author_name, cnt.borrow_count as borrow_count, cat.category as category
from (
select a.name, COUNT(*) as borrow_count
from authors a natural join books b natural join borrowedbooks c
where c.borrow_date <= '2022-09-10' and c.borrow_date >= '2021-09-10'
group by a.name
) as cnt natural join (
select a.name, case when COUNT(*) >= 8 then "High" else "Medium" end as category
from authors a natural join books b natural join borrowedbooks c
group by a.name
) as cat
order by cnt.borrow_count desc, cnt.name desc;
