(define (domain Button-3)
	(:requirements :strips :typing :disjunctive-preconditions)
	(:types
		POS AGENT Button PUSH
	)
	(:constants
		p1_1 p1_2 p2_1 p2_2 - pos
		b0 b1 - Button
		a1 a2 - agent
	)

	(:predicates
		(adj ?i ?j - pos)
		(agent_at ?a - agent ?i - pos)
		(Button_at ?b - Button ?i - pos)
		(Button_pushed ?b - Button)
		(green ?b - Button)
		(yellow ?b - Button)
		(diff_agent ?a1 - agent ?a2 - agent)
	)
	(:action move
		:parameters (?start - pos ?end - pos ?a - agent)
		:precondition (and (adj ?start ?end) (agent_at ?a ?start))
		:effect (and (not (agent_at ?a ?start)) (agent_at ?a ?end))
	)

	(:action push
		:parameters (?start - pos ?b - Button ?a - agent)
		:precondition (and (agent_at ?a ?start) (Button_at ?b ?start) (green ?b))
		:effect (and (not (Button_at ?b ?start)) (Button_pushed ?b))
	)

	(:action joint_push
		:parameters (?start - pos ?b - Button ?a1 - agent ?a2 - agent)
		:precondition (and (agent_at ?a1 ?start) (agent_at ?a2 ?start) (Button_at ?b ?start) (yellow ?b) (diff_agent ?a1 ?a2))
		:effect (and (not (Button_at ?b ?start)) (Button_pushed ?b))
	)

	(:action observe_Button
		:parameters (?i - pos ?a - agent ?b - Button)
		:precondition (and (agent_at ?a ?i))
		:observe
		(Button_at ?b ?i)
	)

)