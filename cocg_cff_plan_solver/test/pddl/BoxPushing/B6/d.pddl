(define (domain Box-4)
	(:requirements :strips :typing :disjunctive-preconditions)
	(:types
		pos agent box push
	)
	(:constants
		p1_1 p1_2 p2_1 p2_2 p3_1 p3_2 p4_1 p4_2 p5_1 p5_2 - pos
		b0 b1 b2 - box
		a1 a2 a3 - agent
	)

	(:predicates
		(adj ?i ?j - pos)
		(agent_at ?a - agent ?i - pos)
		(box_at ?b - box ?i - pos)
		(heavy ?b - box)
		(light ?b - box)
		(diff_agent ?a1 - agent ?a2 - agent)
	)
	(:action move
		:parameters (?start - pos ?end - pos ?a - agent)
		:precondition (and (adj ?start ?end) (agent_at ?a ?start))
		:effect (and (not (agent_at ?a ?start)) (agent_at ?a ?end))
	)

	(:action push
		:parameters (?start - pos ?end - pos ?b - box ?a - agent)
		:precondition (and (adj ?start ?end) (agent_at ?a ?start) (box_at ?b ?start) (light ?b))
		:effect (and (not (box_at ?b ?start)) (box_at ?b ?end))
	)

	(:action joint_push
		:parameters (?start - pos ?end - pos ?b - box ?a1 - agent ?a2 - agent)
		:precondition (and (adj ?start ?end) (agent_at ?a1 ?start) (agent_at ?a2 ?start) (box_at ?b ?start) (heavy ?b) (diff_agent ?a1 ?a2))
		:effect (and (not (box_at ?b ?start)) (box_at ?b ?end))
	)

	(:action observe_box
		:parameters (?i - pos ?a - agent ?b - box)
		:precondition (and (agent_at ?a ?i))
		:observe
		(box_at ?b ?i)
	)
)