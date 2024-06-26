(define 
(problem Box-4)
(:domain Box-4)
(:goal (and (box-at b0 p1-2) (box-at b1 p3-2) (box-at b2 p5-2)))
(:init
	(and
    	(agent-at a1 p1-1)
	(agent-at a2 p5-1)
	(agent-at a3 p1-1)
	
	(adj p1-1 p1-2)
	(adj p2-1 p2-2)
	(adj p3-1 p3-2)
	(adj p4-1 p4-2)
	(adj p5-1 p5-2)
	
	(adj p1-2 p1-1)
	(adj p2-2 p2-1)
	(adj p3-2 p3-1)
	(adj p4-2 p4-1)
	(adj p5-2 p5-1)
	
	(adj p1-1 p2-1)
	(adj p2-1 p3-1)
	(adj p3-1 p4-1)
	(adj p4-1 p5-1)
	
	(adj p2-1 p1-1)
	(adj p3-1 p2-1)
	(adj p4-1 p3-1)
	(adj p5-1 p4-1)

	(not (heavy b0))
	(heavy b1)
	(heavy b2)
	
	(oneof (box-at b0 p1-1) (box-at b0 p1-2))
	(oneof (box-at b1 p3-1) (box-at b1 p3-2))
	(oneof (box-at b2 p5-1) (box-at b2 p5-2))
	
	(same-agent a1 a1)
	(same-agent a2 a2)
	(same-agent a3 a3)
	(not (same-agent a1 a2))
	(not (same-agent a2 a1))
	(not (same-agent a1 a3))
	(not (same-agent a3 a1))
	(not (same-agent a3 a2))
	(not (same-agent a2 a3))
	)
)
)
