WITH cte1 AS (
    SELECT 
        A.name AS author_name,
        COUNT(DISTINCT BB.member_id) AS distinct_member_count,
        B.title,
        COUNT(BB.member_id) as borrow_count
    FROM Authors A
    JOIN Books B ON A.author_id = B.author_id
    JOIN BorrowedBooks BB ON B.book_id = BB.book_id
    WHERE BB.borrow_date BETWEEN '2021-09-10' AND '2022-09-10'
    GROUP BY A.name, B.title
),
cte2 AS (
    SELECT *, ROW_NUMBER() OVER(PARTITION BY author_name ORDER BY borrow_count DESC) as rank_num
    FROM cte1   
),
cte3 AS (
  SELECT author_name, MAX(distinct_member_count) as distinct_member_count
  FROM cte2  
  GROUP BY author_name  
  ORDER BY distinct_member_count DESC, author_name ASC  
  LIMIT 3   
)
SELECT  
     cte3.author_name,
     cte3.distinct_member_count,
     cte2.title as top_1_book
FROM cte3 
JOIN cte2 ON cte3.author_name = cte2.author_name AND cte2.rank_num = 1;